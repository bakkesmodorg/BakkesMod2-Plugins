#include "redirectplugin.h"
#include "bakkesmod/utils/parser.h"

BAKKESMOD_PLUGIN(RedirectPlugin, "Redirect plugin", "0.2", PLUGINTYPE_FREEPLAY)

void RedirectPlugin::onLoad()
{
	cvarManager->registerCvar("redirect_shot_speed", "(850, 1100)", "Speed to shoot the ball at", true, true, 0, true, 2000);
	cvarManager->registerCvar("redirect_pass_offset", "50", "Default X/Y offset to shoot ahead");
	cvarManager->registerCvar("redirect_pass_offset_z", "200", "Default Z offset for passing");
	cvarManager->registerCvar("redirect_pass_predict", "1", "Predict where the player will be when the pass arrives", true, true, 0, true, 1);
	cvarManager->registerCvar("redirect_on_ground", "0", "Pass over the floor", true, true, 0, true, 1);

	cvarManager->registerCvar("redirect_predict_multiplier_x", "2", "X offset to use when passing (playerVelocityX * value)");
	cvarManager->registerCvar("redirect_predict_multiplier_y", "2", "Y offset to use when passing (playerVelocityY * value)");

	cvarManager->registerNotifier("redirect_shoot",  [&cvarManager = this->cvarManager, &gameWrapper = this->gameWrapper](vector<string> command) {
		if (!gameWrapper->IsInTutorial())
			return;
		TutorialWrapper training = gameWrapper->GetGameEventAsTutorial();

		Vector playerPosition = training.GetGameCar().GetLocation();
		Vector ballPosition = training.GetBall().GetLocation();
		Vector playerVelocity = training.GetGameCar().GetVelocity();
		
		
		float ballSpeed = cvarManager->getCvar("redirect_shot_speed").getIntValue();
		float offset = cvarManager->getCvar("redirect_pass_offset").getIntValue();
		float offset_z = cvarManager->getCvar("redirect_pass_offset_z").getIntValue();
		
		bool predict = cvarManager->getCvar("redirect_pass_predict").getBoolValue();


		float predictMultiplierX = cvarManager->getCvar("redirect_predict_multiplier_x").getFloatValue();
		float predictMultiplierY = cvarManager->getCvar("redirect_predict_multiplier_y").getFloatValue();

		bool onGround = cvarManager->getCvar("redirect_on_ground").getBoolValue();
		
		int offsetX = random(0, (int)offset);
		int offsetY = random(0, (int)offset);
		int offsetZ = random(offset_z / 3, offset_z);
		Vector offsetVec = Vector(offsetX, offsetY, offsetZ);
		
		Vector velMultiplied;
		if (predict)
			velMultiplied = playerVelocity * Vector(predictMultiplierX, predictMultiplierY, 1);
		offsetVec = offsetVec + velMultiplied;
		Vector shotData = training.GenerateShot(ballPosition, playerPosition + offsetVec, ballSpeed);
		if (onGround)
			shotData.Z = 0;
		training.GetBall().SetVelocity(shotData);
	});
}

void RedirectPlugin::onUnload()
{
	
}