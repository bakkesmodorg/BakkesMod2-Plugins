#include "BallPredictionPlugin.h"
#include "bakkesmod/wrappers/GameEvent/ServerWrapper.h"
#include "bakkesmod/wrappers/GameObject/BallWrapper.h"

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
	predictSteps = make_shared<int>(15);
	predictStepSize = make_shared<float>(.1f);
	cvarManager->registerCvar("cl_soccar_predictball", "0", "Show ball prediction", true, true, 0, true, 1).bindTo(predictOn);
	cvarManager->registerCvar("cl_soccar_predictball_steps", "15", "Predict ball steps", true, true, 0, true, 100).bindTo(predictSteps);
	cvarManager->registerCvar("cl_soccar_predictball_stepsize", ".1", "Predict ball stepsize", true, true, 0, true, 1).bindTo(predictStepSize);
	gameWrapper->RegisterDrawable(std::bind(&BallPredictionPlugin::Render, this, std::placeholders::_1));
}

void BallPredictionPlugin::Render(CanvasWrapper canvas)
{
	if (*predictOn && gameWrapper->IsInGame())
	{
		ServerWrapper game = gameWrapper->GetGameEventAsServer();
		BallWrapper ball = game.GetBall();
		if (ball.IsNull()) //Ball might be destroyed, for example between scoring and goal replay
			return;
		
		Vector2 currentBallLocation2D = canvas.Project(ball.GetLocation()); //Project ball current 3d location to 2d screen
		PredictionInfo predictionInfo;

		for (int i = 0; i < *predictSteps; i++) {
			predictionInfo = ball.PredictPosition(i * (*predictStepSize));
			Vector2 newPredictedLocation2D = canvas.Project(predictionInfo.Location);

			LineColor stepColor = colors[i % 2];
			canvas.SetColor(stepColor.r, stepColor.g, stepColor.b, stepColor.a);
			canvas.DrawLine(currentBallLocation2D, newPredictedLocation2D);
			
			currentBallLocation2D = newPredictedLocation2D;
		}
		
	}
}

void BallPredictionPlugin::onUnload()
{
}
