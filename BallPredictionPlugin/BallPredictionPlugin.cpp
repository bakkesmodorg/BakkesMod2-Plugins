#define LINMATH_H //Conflicts with linmath.h if we done declare this here
#include "BallPredictionPlugin.h"
#include "bakkesmod/wrappers/GameEvent/ServerWrapper.h"
#include "bakkesmod/wrappers/GameObject/BallWrapper.h"
#include "ball.h"

BAKKESMOD_PLUGIN(BallPredictionPlugin, "Ball prediction plugin", "0.1", PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING)

BallPredictionPlugin::BallPredictionPlugin()
{
}

BallPredictionPlugin::~BallPredictionPlugin()
{
}

void BallPredictionPlugin::onLoad()
{
	predictOn = make_shared<bool>(true);
	predictSteps = make_shared<int>(480);
	predictStepSize = make_shared<float>(40.f);
	cvarManager->registerCvar("cl_soccar_predictball", "0", "Show ball prediction", true, true, 0, true, 1).bindTo(predictOn);

	cvarManager->registerCvar("cl_soccar_predictball_steps", "480", "Predict ball steps", true, true, 0, true, 1000).bindTo(predictSteps);
	cvarManager->registerCvar("cl_soccar_predictball_stepsize", "40", "Predict ball stepsize", true, true, 0, true, 1000).bindTo(predictStepSize);

	cvarManager->getCvar("cl_soccar_predictball").addOnValueChanged(std::bind(&BallPredictionPlugin::OnPredictOnValueChanged, this, std::placeholders::_1, std::placeholders::_2));

	//cvarManager->getCvar("cl_soccar_predictball_stepsize").setValue((1.f / 61.f));
	gameWrapper->HookEvent("Function TAGame.Mutator_Freeplay_TA.Init", bind(&BallPredictionPlugin::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed", bind(&BallPredictionPlugin::OnFreeplayDestroy, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_TrainingEditor_TA.StartPlayTest", bind(&BallPredictionPlugin::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_TrainingEditor_TA.Destroyed", bind(&BallPredictionPlugin::OnFreeplayDestroy, this, std::placeholders::_1));
}

void BallPredictionPlugin::OnFreeplayLoad(std::string eventName)
{
	if (*predictOn)
	{
		gameWrapper->RegisterDrawable(std::bind(&BallPredictionPlugin::Render, this, std::placeholders::_1));
		gameWrapper->HookEvent("Function TAGame.Car_TA.SetVehicleInput", bind(&BallPredictionPlugin::Predict, this, std::placeholders::_1));
	}
}

void BallPredictionPlugin::OnFreeplayDestroy(std::string eventName)
{
	gameWrapper->UnregisterDrawables();
	gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
}

void BallPredictionPlugin::OnPredictOnValueChanged(std::string oldValue, CVarWrapper cvar)
{
	if (cvar.getBoolValue() && gameWrapper->IsInGame())
	{
		OnFreeplayLoad("Load");
		//gameWrapper->RegisterDrawable(std::bind(&BallPredictionPlugin::Render, this, std::placeholders::_1));
	}
	else
	{
		OnFreeplayDestroy("Destroy");
		//gameWrapper->UnregisterDrawables();
	}
}

/*
// discard time stamp
ss >> t;

// position
ss >> s.x[0];
ss >> s.x[1];
ss >> s.x[2];

// velocity
ss >> s.v[0];
ss >> s.v[1];
ss >> s.v[2];

// discard euler angle info
ss >> t;
ss >> t;
ss >> t;

// angular velocity
ss >> s.w[0];
ss >> s.w[1];
ss >> s.w[2];

*/
#include "vec.h"
#include "DynamicState.h"
#include <iostream> // std::cout
#include <fstream>

/*
Predict is called when the setvehicleinput event is fired, this is because
setvehicleinput is synced up with the physics ticks, giving us accurate predictions
and allowing us to not having to predict every render frame
*/
int currentStep = 0;
void BallPredictionPlugin::Predict(std::string eventName)
{
	if (!gameWrapper->IsInGame()) {
		return;
	}
	ServerWrapper game = gameWrapper->GetGameEventAsServer();
	BallWrapper ball = game.GetBall();
	if (ball.IsNull()) //Ball might be destroyed, for example between scoring and goal replay
		return;

	//Load ball information into chips ball format

	//Clear previously created predicted points
	//predictedPoints.clear();
	//predictedPoints.push_back({ location, false });
	int startIdx = currentStep * (*predictStepSize);
	Ball chipBall;
	bool lastDirection[3];
	if (currentStep != 0)
	{
		startIdx -= 1;
		PredictedPoint pp = predictedPoints[startIdx];
		chipBall.x[0] = pp.location.X;
		chipBall.x[1] = pp.location.Y;
		chipBall.x[2] = pp.location.Z;

		chipBall.v[0] = pp.velocity.X;
		chipBall.v[1] = pp.velocity.Y;
		chipBall.v[2] = pp.velocity.Z;

		chipBall.w[0] = pp.angVel.X;
		chipBall.w[1] = pp.angVel.Y;
		chipBall.w[2] = pp.angVel.Z;
		lastDirection[0] = pp.velocity.X >= 0;
		lastDirection[1] = pp.velocity.Y >= 0;
		lastDirection[2] = pp.velocity.Z >= 0;
	}
	else
	{
		//Get ball info from game
		Vector location = ball.GetLocation();
		Vector velocity = ball.GetVelocity();
		Vector angVel = ball.GetAngularVelocity();
		predictedPoints[0] = {location, false};
		chipBall.x[0] = location.X;
		chipBall.x[1] = location.Y;
		chipBall.x[2] = location.Z;

		chipBall.v[0] = velocity.X;
		chipBall.v[1] = velocity.Y;
		chipBall.v[2] = velocity.Z;

		chipBall.w[0] = angVel.X;
		chipBall.w[1] = angVel.Y;
		chipBall.w[2] = angVel.Z;
		lastDirection[0] = velocity.X >= 0;
		lastDirection[1] = velocity.Y >= 0;
		lastDirection[2] = velocity.Z >= 0;
	}
	//Store velocities to later determine bounces/apices

	//Step X physics ticks ahead (determined by cvar cl_soccar_predictball_steps
	int i = 0;
	int max = (currentStep * (*predictStepSize) + (*predictStepSize));
	for (i = startIdx + 1; i < max; i++)
	{
		bool isApex = false;
		//Chips example is 1/61, but setvehicleinputs is tied to physics tickrate @ 120
		chipBall.step((1.f / 61.f) * .5f);
		chipBall.step((1.f / 61.f) * .5f);
		bool currentDirection[3] = {chipBall.v[0] >= 0, chipBall.v[1] >= 0, chipBall.v[2] >= 0};
		Vector apexLocation = {0, 0, 0};
		if (currentDirection[0] != lastDirection[0])
		{
			isApex = true;
			apexLocation.X = 93 * ((lastDirection[0]) ? 1 : -1);
			lastDirection[0] = currentDirection[0];
		}
		if (currentDirection[1] != lastDirection[1])
		{
			isApex = true;
			apexLocation.Y = 93 * ((lastDirection[1]) ? 1 : -1);
			lastDirection[1] = currentDirection[1];
		}
		if (currentDirection[2] != lastDirection[2])
		{
			isApex = true;
			apexLocation.Z = 93 * ((lastDirection[2]) ? 1 : -1);
			lastDirection[2] = currentDirection[2];
		}
		predictedPoints[i] = {{chipBall.x[0], chipBall.x[1], chipBall.x[2]}, isApex, apexLocation};
	}
	predictedPoints[i - 1].velocity = {chipBall.v[0], chipBall.v[1], chipBall.v[2]};
	predictedPoints[i - 1].angVel = {chipBall.w[0], chipBall.w[1], chipBall.w[2]};
	currentStep++;
	if (currentStep >= (*predictSteps) / (*predictStepSize))
	{
		currentStep = 0;
	}
}

bool b = false;
const int max_apex = 5;
void BallPredictionPlugin::Render(CanvasWrapper canvas)
{
	if (*predictOn && gameWrapper->IsInGame())
	{

		Vector2 currentBallLocation2D = canvas.Project(predictedPoints[0].location); //Project ball current 3d location to 2d screen
		int apexes = 0;
		for (int i = 1; i < (*predictSteps); i++)
		{
			PredictedPoint p = predictedPoints[i];
			Vector2 newPredictedLocation2D = canvas.Project(p.location);

			if (currentBallLocation2D.X < 0 || currentBallLocation2D.X > canvas.GetSize().X || currentBallLocation2D.Y < 0 || currentBallLocation2D.Y > canvas.GetSize().Y)
				continue;

			LineColor stepColor = colors[0];

			canvas.SetColor(stepColor.r, stepColor.g, stepColor.b, stepColor.a);
			canvas.DrawLine(currentBallLocation2D, newPredictedLocation2D, 3);

			stepColor = colors[1];
			canvas.SetColor(stepColor.r, stepColor.g, stepColor.b, stepColor.a);
			canvas.DrawLine(currentBallLocation2D.minus({2, 2}), newPredictedLocation2D.minus({2, 2}), 2);
			canvas.DrawLine(currentBallLocation2D.minus({-2, -2}), newPredictedLocation2D.minus({-2, -2}), 2);
			if (p.isApex && apexes < max_apex)
			{
				apexes++;
				canvas.SetColor(255, 0, 0, 200);
				Vector2 predictedBounceLocation = canvas.Project(p.location + p.apexLocation);

				canvas.SetPosition(predictedBounceLocation.minus({5, 5}));
				canvas.FillBox({10, 10});
			}
			//myfile << chipBall.x[0] << "," << chipBall.x[1] << "," << chipBall.x[2] << predictionInfo.;
			currentBallLocation2D = newPredictedLocation2D;
		}
	}
}

void BallPredictionPlugin::onUnload()
{
}
