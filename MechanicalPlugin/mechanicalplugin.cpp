#include "MechanicalPlugin.h"
#include "bakkesmod\wrappers\tutorialwrapper.h"
#include "bakkesmod\wrappers\ballwrapper.h"
#include "bakkesmod\wrappers\gameeventwrapper.h"
using namespace std::placeholders;

BAKKESMOD_PLUGIN(MechanicalPlugin, "Mechanical training plugin", "0.1", PLUGINTYPE_FREEPLAY)

template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}

void MechanicalPlugin::onLoad()
{
	limitSteer = make_shared<float>(1.f);
	limitThrottle = make_shared<float>(1.f);
	limitPitch = make_shared<float>(1.f);
	limitYaw = make_shared<float>(1.f);
	limitRoll = make_shared<float>(1.f);

	disableHandbrake = make_shared<bool>(false);
	disableJump = make_shared<bool>(false);
	disableBoost = make_shared<bool>(false);

	cvarManager->registerCvar("mech_limit_steer", "1", "Clamps steer", true, true, 0.f, true, 1.f).bindTo(limitSteer);
	cvarManager->registerCvar("mech_limit_throttle", "1", "Clamps throttle", true, true, 0.f, true, 1.f).bindTo(limitThrottle);
	cvarManager->registerCvar("mech_limit_yaw", "1", "Clamps yaw", true, true, 0.f, true, 1.f).bindTo(limitYaw);
	cvarManager->registerCvar("mech_limit_pitch", "1", "Clamps pitch", true, true, 0.f, true, 1.f).bindTo(limitYaw);
	cvarManager->registerCvar("mech_limit_roll", "1", "Clamps roll", true, true, 0.f, true, 1.f).bindTo(limitRoll);

	cvarManager->registerCvar("mech_disable_handbrake", "0", "Disables handbrake", true, true, 0.f, true, 1.f).bindTo(disableHandbrake);
	cvarManager->registerCvar("mech_disable_jump", "0", "Disables jump", true, true, 0.f, true, 1.f).bindTo(disableJump);
	cvarManager->registerCvar("mech_disable_boost", "0", "Disables boost", true, true, 0.f, true, 1.f).bindTo(disableBoost);

	gameWrapper->HookEvent("Function TAGame.PlayerController_TA.PrePhysicsStep", bind(&MechanicalPlugin::OnPreAsync, this, _1));
}

void MechanicalPlugin::onUnload()
{
}

void MechanicalPlugin::OnPreAsync(std::string funcName)
{
	if (gameWrapper->IsInTutorial())
	{
		auto players = gameWrapper->GetGameEventAsServer().GetPlayers();
		for (int i = 0; i < players.Count(); i++)
		{
			auto player = players.Get(i);
			ControllerInput ci = player.GetInput();
			if (*disableHandbrake)
			{
				ci.Handbrake = false;
			}
			if (*disableBoost) 
			{
				ci.ActivateBoost = false;
				ci.HoldingBoost = false;
			}

			if (*disableJump) 
			{
				ci.Jump = false;
				ci.Jumped = false;
			}

			ci.Steer = clip(ci.Steer, -abs(*limitSteer), abs(*limitSteer));
			ci.Throttle = clip(ci.Throttle, -abs(*limitThrottle), abs(*limitThrottle));
			ci.Yaw = clip(ci.Yaw, -abs(*limitYaw), abs(*limitYaw));
			ci.Pitch = clip(ci.Pitch, -abs(*limitPitch), abs(*limitPitch));
			ci.Roll = clip(ci.Roll, -abs(*limitRoll), abs(*limitRoll));
			player.SetInput(ci);
		}
	}
}
