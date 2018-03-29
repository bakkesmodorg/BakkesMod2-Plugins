#include "ReboundPlugin.h"
#include "bakkesmod\wrappers\ArrayWrapper.h"
#include "bakkesmod\wrappers\GameEvent\TutorialWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarWrapper.h"
#include "bakkesmod\wrappers\GameObject\BallWrapper.h"
#include <math.h>

BAKKESMOD_PLUGIN(ReboundPlugin, "Rebound plugin", "0.1", PLUGINTYPE_FREEPLAY)

void ReboundPlugin::onLoad()
{
	cvarManager->registerCvar("rebound_shotspeed", "780", "Shotspeed to use for rebounds", true, true, 0, true, 2000);
	cvarManager->registerCvar("rebound_addedheight", "(300, 1400)", "Height above the backboard to shoot", true, true, -5000, true, 10000);
	cvarManager->registerCvar("rebound_side_offset", "(0, 500)", "Variance for backboard shots (0 = only above net)", true, true, -5000, true, 5000);
	cvarManager->registerCvar("rebound_addedspin", "(-6, 6)", "Added rotational velocity to give when doing a backboard shot", true, true, -50000, true, 50000);
	cvarManager->registerCvar("rebound_resetspin", "0", "Whether or not to remove the existing spin on the ball", true, true, 0, true, 1);

	cvarManager->registerNotifier("rebound_shoot", [&cm = this->cvarManager, &gw = this->gameWrapper](vector<string>) {
		if (!gw->IsInTutorial())
			return;
		TutorialWrapper tw = gw->GetGameEventAsTutorial();

		//If there are less than 2 goals, its probably a workshop map, dont rebound here
		if (tw.GetGoals().Count() < 2)
			return;

		Vector ballLoc = tw.GetBall().GetLocation();

		//Calculate nearest goal
		CarWrapper player = tw.GetGameCar();
		Vector playerLocLater = tw.GetGameCar().GetLocation() + (tw.GetGameCar().GetVelocity() * 200); //Calculate where player is facing
		if (abs(player.GetVelocity().X) < 1 && abs(player.GetVelocity().X) < 1) 
		{
			playerLocLater = ballLoc; //if player is not moving, set rebound to the goal the ball is closest to
		}
		Vector goal1Diff = tw.GetGoalLocation(0) - playerLocLater;
		Vector goal2Diff = tw.GetGoalLocation(1) - playerLocLater;
		float goal1DiffF = abs(goal1Diff.X) + abs(goal1Diff.Y) + abs(goal1Diff.Z);
		float goal2DiffF = abs(goal2Diff.X) + abs(goal2Diff.Y) + abs(goal2Diff.Z);

		int target = 0;
		if (goal1DiffF > goal2DiffF)
		{
			target = 1;
		}
		float sideOffset = cm->getCvar("rebound_side_offset").getFloatValue();

		Vector goalTarget = tw.GenerateGoalAimLocation(target, ballLoc);

		goalTarget.Z = tw.GetGoalExtent(target).Z + cm->getCvar("rebound_addedheight").getFloatValue(); 
		goalTarget.X += sideOffset;

		float reboundSpeed = cm->getCvar("rebound_shotspeed").getFloatValue();

		Vector shot = tw.GenerateShot(ballLoc, goalTarget, reboundSpeed);
		if (cm->getCvar("rebound_resetspin").getBoolValue()) {
			tw.GetBall().Stop();
		}
		Vector addedSpin = { cm->getCvar("rebound_addedspin").getFloatValue(),
			cm->getCvar("rebound_addedspin").getFloatValue(),
			cm->getCvar("rebound_addedspin").getFloatValue() };

		tw.GetBall().SetVelocity(shot);
		tw.GetBall().SetAngularVelocity(addedSpin, true);
	});
}

void ReboundPlugin::onUnload()
{
}
