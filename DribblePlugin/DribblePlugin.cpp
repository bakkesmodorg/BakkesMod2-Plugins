#include "DribblePlugin.h"
#include "bakkesmod\wrappers\GameEvent\TutorialWrapper.h"
#include "bakkesmod\wrappers\GameObject\BallWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarWrapper.h"

BAKKESMOD_PLUGIN(DribblePlugin, "Dribble plugin", "0.1", PLUGINTYPE_FREEPLAY)

void DribblePlugin::onLoad()
{
	//Place the ball on top of the player
	cvarManager->registerNotifier("ballontop", [&gw = this->gameWrapper](vector<string> commands) {
		if (!gw->IsInTutorial())
			return;
		TutorialWrapper tutorial = gw->GetGameEventAsTutorial();
		BallWrapper ball = tutorial.GetBall();
		CarWrapper car = tutorial.GetGameCar();

		Vector playerVelocity = car.GetVelocity();
		Vector addToBall = Vector(playerVelocity.X, playerVelocity.Y, 170);

		addToBall.X = max(min(20.0f, addToBall.X), -20.0f);//maybe limit the X a bit more
		addToBall.Y = max(min(30.0f, addToBall.Y), -30.0f);

		ball.SetLocation(car.GetLocation() + addToBall);
		ball.SetVelocity(playerVelocity);
	});

	
	

	//Experimental training for shooting the ball somewhere near the player (helps with practicing car orientation and quick thinking)
	cvarManager->registerCvar("shootatme_bounds_x", "(-200, 200)");
	cvarManager->registerCvar("shootatme_bounds_y", "(-200, 200)");
	cvarManager->registerCvar("shootatme_bounds_z", "(-400, 400)");
	cvarManager->registerCvar("shootatme_speed", "(800, 1000)");

	cvarManager->registerNotifier("shootatme", [&cm = this->cvarManager, &gw = this->gameWrapper](vector<string> commands) {
		if (!gw->IsInTutorial())
			return;
		TutorialWrapper tutorial = gw->GetGameEventAsTutorial();
		BallWrapper ball = tutorial.GetBall();
		CarWrapper car = tutorial.GetGameCar();
		Vector location = car.GetLocation();
		location = location + Vector(cm->getCvar("shootatme_bounds_x").getFloatValue(), 
										cm->getCvar("shootatme_bounds_y").getFloatValue(), 
										cm->getCvar("shootatme_bounds_z").getFloatValue());
		Vector shot = tutorial.GenerateShot(ball.GetLocation(), location, cm->getCvar("shootatme_speed").getFloatValue());
		ball.SetVelocity(shot);
	});
}

void DribblePlugin::onUnload()
{
}

