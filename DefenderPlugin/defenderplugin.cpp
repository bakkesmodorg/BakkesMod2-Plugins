#include "defenderplugin.h"
#include "utils\parser.h"

#include "bakkesmod\wrappers\tutorialwrapper.h"

BAKKESMOD_PLUGIN(DefenderPlugin, "Defender plugin", "0.1", PLUGINTYPE_FREEPLAY)


void DefenderPlugin::onLoad()
{
	cvarManager->registerCvar("defender_shotspeed", "(800, 1100)", "The speed to use when using defender training", true, true, 0.f, true, 1999.9);
	cvarManager->registerCvar("defender_cooldown", "(3000, 6000)", "Time to wait after shooting/saving a ball", true, true, 0.f, true, 120000.f);

	cvarManager->registerNotifier("defender_start", [this](std::vector<string> params) {
		if (!gameWrapper->IsInTutorial())
		{
			cvarManager->log("You need to be in freeplay to use this plugin.");
			return;
		}
		cvarManager->executeCommand("cl_freeplay_enablegoal 0");
		this->defenderEnabled = true;
		this->CheckForShot();
	});

	cvarManager->registerNotifier("defender_stop", [this](std::vector<string> params) {
		this->defenderEnabled = false;
	});

}

void DefenderPlugin::onUnload()
{
	this->defenderEnabled = false;
}

void DefenderPlugin::CheckForShot()
{
	if (!defenderEnabled || !gameWrapper->IsInTutorial())
		return; //Player stopped defender training or left freeplay

	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		this->CheckForShot();
	}, this->HandleShot());
}

float DefenderPlugin::HandleShot()
{
	if (!gameWrapper->IsInTutorial() || !defenderEnabled)
		return .5f;
	TutorialWrapper training = gameWrapper->GetGameEventAsTutorial();
	BallWrapper b = training.GetBall();
	float touchTime = b.GetLastTouchTime();

	float coolDown = cvarManager->getCvar("defender_cooldown").getFloatValue() / 1000;

	//if scored
	if (training.IsInGoal(b.GetLocation()))
	{
		b.Stop();
		b.SetLocation(Vector(random(-2000.0f, 2000.0f), random(400.0f, 400.0f), random(140.0f, 600.0f)));
		b.Stop();
		return coolDown;
	}
	//If ball is already/still moving towards goal, player still has to save.
	if (training.IsBallMovingTowardsGoal(destinationGoal, b)) {
		return .75f;
	}

	//Player recently touched the ball
	if (touchTime != lastTouchTime)
	{
		currentTouchChecks = -1;
		lastTouchTime = touchTime;
		return coolDown;
	}
	else if (currentTouchChecks >= touchTimeout || currentTouchChecks == -1) //-1 for after touch 
	{
		currentTouchChecks = 0;
		ExecuteShot();
	}
	else {
		currentTouchChecks++;
	}
	return .75;
}

void DefenderPlugin::ExecuteShot()
{
	if (!gameWrapper->IsInTutorial() || !defenderEnabled)
		return;
	TutorialWrapper training = gameWrapper->GetGameEventAsTutorial();
	BallWrapper b = training.GetBall();

	float coolDown = cvarManager->getCvar("defender_cooldown").getFloatValue() / 1000;

	//touchTimeout = coolDown / 1000;
	touchTimeout = random(4, 6);
	float shotSpeed = cvarManager->getCvar("defender_shotspeed").getFloatValue();

	Vector target = training.GenerateGoalAimLocation(destinationGoal, b.GetLocation());
	Vector shot = training.GenerateShot(b.GetLocation(), target, shotSpeed);
	b.Stop();
	b.SetVelocity(shot);
}
