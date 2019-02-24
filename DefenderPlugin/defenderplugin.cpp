#include "defenderplugin.h"
#include "bakkesmod/wrappers/GameEvent/TutorialWrapper.h"
#include "bakkesmod/wrappers/GameObject/BallWrapper.h"
#include "utils/parser.h"
#include <string.h>

BAKKESMOD_PLUGIN(DefenderPlugin, "Defender plugin", "0.1", PLUGINTYPE_FREEPLAY)


void DefenderPlugin::onLoad()
{
	cvarManager->registerCvar("defender_shotspeed", "(800, 1100)", "The speed to use when using defender training", true, true, 0.f, true, 1999.9);
	cvarManager->registerCvar("defender_cooldown", "(3000, 6000)", "Time to wait after shooting/saving a ball", true, true, 0.f, true, 120000.f);
	cvarManager->registerCvar("defender_difficulty", "8", "", true, true, 0.f, true, 1.f);
	cvarManager->registerCvar("defender_active", "0", "Whether defender plugin is active", true, true, 0.f, true, 1.f).bindTo(pluginActive);
	cvarManager->getCvar("defender_active").addOnValueChanged(std::bind(&DefenderPlugin::OnDefenderStatusChanged, this, std::placeholders::_1, std::placeholders::_2));

	/*cvarManager->registerNotifier("defender_shoot", [this](std::vector<string> params) {
		defenderEnabled = true;
		ExecuteShot();
		defenderEnabled = false;
	}, "Shoots a single defender plugin shot", PERMISSION_FREEPLAY | PERMISSION_PAUSEMENU_CLOSED);*/


	//Keep these for backwards compatibility. Prefered use is defender_active now
	cvarManager->registerNotifier("defender_start", [this](std::vector<string> params) {
		cvarManager->executeCommand("defender_active 1");
	}, "Starts defender training (needs to be in freeplay in order to execute)", PERMISSION_FREEPLAY);

	cvarManager->registerNotifier("defender_stop", [this](std::vector<string> params) {
		cvarManager->executeCommand("defender_active 0");
	}, "Stops defender training", PERMISSION_ALL);

	gameWrapper->HookEvent("Function GameEvent_Tutorial_TA.Active.HandleHitGoal", std::bind(&DefenderPlugin::OnGoalScored, this, std::placeholders::_1));
}

void DefenderPlugin::onUnload()
{
	cvarManager->executeCommand("defender_active 0"); 
}

float DefenderPlugin::GenerateNextTimeout()
{
	if (gameWrapper->IsInFreeplay()) {
		nextShotTimeout = gameWrapper->GetGameEventAsServer().GetSecondsElapsed() + cvarManager->getCvar("defender_cooldown").getFloatValue() / 1000.f;
	}
	return nextShotTimeout;
}

void DefenderPlugin::CheckForShot()
{
	if (!gameWrapper->IsInFreeplay())
	{
		cvarManager->executeCommand("defender_active 0");
		return; //Player stopped defender training or left freeplay
	}
	if (!*pluginActive)
	{
		return; //User stopped the plugin
	}

	this->HandleShot();

	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		this->CheckForShot();
	}, .100f);
}

void DefenderPlugin::HandleShot()
{
	ServerWrapper training = gameWrapper->GetGameEventAsServer();
	BallWrapper bw = training.GetBall();
	if (bw.IsNull()) //Ball has exploded, user toggled goal scoring on manually probably, wait till ball respawned
	{
		return;
	}
	BallWrapper b = training.GetBall();
	float touchTime = b.GetLastTouchTime();
	//cvarManager->log("SE " + to_string(training.GetSecondsElapsed()) + ", timeout: " + to_string(nextShotTimeout));
	if (training.GetSecondsElapsed() - nextShotTimeout < 0) //Have not hit timeout yet, wait
	{
		return;
	}

	//If ball is already/still moving towards goal, player still has to save.
	if (training.IsBallMovingTowardsGoal(destinationGoal, b)) {
		GenerateNextTimeout(); //Reset the timeout.
		return;
	}

	//If we reached this point, we can take another shot
	float shotSpeed = cvarManager->getCvar("defender_shotspeed").getFloatValue();

	if (training.GetGoals().Count() <= destinationGoal) { //Player is in workshop or something? Need to abort everything here
		cvarManager->log("Unable to generate shot, make sure you're in freeplay!");
		cvarManager->executeCommand("defender_active 0");
		return;
	}

	GoalWrapper goal = training.GetGoals().Get(destinationGoal);
	
	Vector goalLocation = Vector(0, -5120.f, 0);
	Vector goalExtend = Vector(893.f, -300.f, 643);//Get rid of posts/crossbar
	cvarManager->log("Extend: " + to_string(goalLocation.X) + ", " + to_string(goalLocation.Y) + ", " + to_string(goalLocation.Z));
	Vector aimLocation = goalLocation + (goalExtend * Vector(random(cvarManager->getCvar("defender_difficulty").getFloatValue() / 10.f, 1.f) * (random(0.f, 1.f) > .5f ? 1 : -1),
		1.f,
		random(cvarManager->getCvar("defender_difficulty").getFloatValue() / 10.f, .95f)));
	CarWrapper car = training.GetGameCar();
	if (!car.IsNull())
	{

	}

	//Vector target = training.GenerateGoalAimLocation(destinationGoal, b.GetLocation());
	Vector shot = training.GenerateShot(b.GetLocation(), aimLocation, shotSpeed);
	b.Stop();
	b.SetVelocity(shot);
	GenerateNextTimeout();
}



void DefenderPlugin::OnGoalScored(std::string eventName)
{
	if (!*pluginActive) { return; }
	ServerWrapper tutorial = gameWrapper->GetGameEventAsServer(); //Event is from Tutorial_TA, so we know this exists
	BallWrapper ball = tutorial.GetBall(); 
	if (ball.IsNull()) //Ball hit goal, so we know A ball exists, but we dont neccesarily know whether its the game ball or not
		return;
	ball.Stop();
	ball.SetLocation(Vector(random(-2000.0f, 2000.0f), random(400.0f, 400.0f), random(140.0f, 600.0f))); //Respawn ball somewhere else on the field
	ball.Stop();
}

void DefenderPlugin::OnHitBall(std::string eventName)
{
	if (!gameWrapper->IsInFreeplay()) return; // Should never occur, but just incase I messed up my unhooking logic
	ServerWrapper tutorial = gameWrapper->GetGameEventAsServer();
	lastTouchTime = tutorial.GetSecondsElapsed();
	GenerateNextTimeout();
}

void DefenderPlugin::OnDefenderStatusChanged(std::string oldValue, CVarWrapper newValue)
{
	if (newValue.getBoolValue())
	{
		if (!gameWrapper->IsInFreeplay())
		{
			cvarManager->executeCommand("defender_active 0");
			return;
		}
		cvarManager->executeCommand("sv_freeplay_enablegoal 0");
		gameWrapper->HookEventPost("Function TAGame.Car_TA.EventHitBall", std::bind(&DefenderPlugin::OnHitBall, this, std::placeholders::_1));
		GenerateNextTimeout();
		CheckForShot();
	}
	else
	{
		gameWrapper->UnhookEvent("Function TAGame.Car_TA.EventHitBall");
	}
}
