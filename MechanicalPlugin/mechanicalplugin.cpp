#include "MechanicalPlugin.h"
#include "bakkesmod\wrappers\GameEvent\TutorialWrapper.h"
#include "bakkesmod\wrappers\GameObject\BallWrapper.h"
#include "bakkesmod\wrappers\GameObject\CarWrapper.h"
#include "bakkesmod\wrappers\ArrayWrapper.h"
using namespace std::placeholders;

BAKKESMOD_PLUGIN(MechanicalPlugin, "Mechanical training plugin", "0.1", PLUGINTYPE_FREEPLAY)

template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}

void MechanicalPlugin::onLoad()
{
	enabled = make_shared<bool>(false);
	limitSteer = make_shared<float>(1.f);
	limitThrottle = make_shared<float>(1.f);
	limitPitch = make_shared<float>(1.f);
	limitYaw = make_shared<float>(1.f);
	limitRoll = make_shared<float>(1.f);

	disableHandbrake = make_shared<bool>(false);
	disableJump = make_shared<bool>(false);
	disableBoost = make_shared<bool>(false);
	holdBoost = make_shared<bool>(false);
	gameWrapper->HookEvent("Function TAGame.GameEvent_Tutorial_TA.OnInit", bind(&MechanicalPlugin::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Tutorial_TA.Destroyed", bind(&MechanicalPlugin::OnFreeplayDestroy, this, std::placeholders::_1));

	cvarManager->registerCvar("mech_enabled", "0", "Enables/disable mechanical steer functionality", true, true, 0.f, true, 1.f)
		.addOnValueChanged(std::bind(&MechanicalPlugin::OnEnabledChanged, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->getCvar("mech_enabled").bindTo(enabled);
	cvarManager->registerCvar("mech_steer_limit", "1", "Clamps steer", true, true, 0.f, true, 1.f).bindTo(limitSteer);
	cvarManager->registerCvar("mech_throttle_limit", "1", "Clamps throttle", true, true, 0.f, true, 1.f).bindTo(limitThrottle);
	cvarManager->registerCvar("mech_yaw_limit", "1", "Clamps yaw", true, true, 0.f, true, 1.f).bindTo(limitYaw);
	cvarManager->registerCvar("mech_pitch_limit", "1", "Clamps pitch", true, true, 0.f, true, 1.f).bindTo(limitYaw);
	cvarManager->registerCvar("mech_roll_limit", "1", "Clamps roll", true, true, 0.f, true, 1.f).bindTo(limitRoll);

	cvarManager->registerCvar("mech_disable_handbrake", "0", "Disables handbrake", true, true, 0.f, true, 1.f).bindTo(disableHandbrake);
	cvarManager->registerCvar("mech_disable_jump", "0", "Disables jump", true, true, 0.f, true, 1.f).bindTo(disableJump);
	cvarManager->registerCvar("mech_disable_boost", "0", "Disables boost", true, true, 0.f, true, 1.f).bindTo(disableBoost);

	cvarManager->registerCvar("mech_hold_boost", "0", "Holds boost", true, true, 0.f, true, 1.f).bindTo(holdBoost);
	/*gameWrapper->HookEventWithCaller<CarWrapper>("Function TAGame.Car_TA.SetVehicleInput",
		bind(&MechanicalPlugin::OnPreAsync, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));*/

	//gameWrapper->HookEventWithCallerPost<PlayerControllerWrapper >("Function TAGame.PlayerController_TA.PlayerMove", std::bind(&MechanicalPlugin::OnPreAsync, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	// > HookEvent("Function TAGame.RBActor_TA.PreAsyncTick", bind(&MechanicalPlugin::OnPreAsync, this, _1));
}

void MechanicalPlugin::onUnload()
{
}

void MechanicalPlugin::OnPreAsync(CarWrapper cw, void * params, string funcName)
{
	//cvarManager->log("PreAsync");
	if (gameWrapper->IsInTutorial())
	{
		//cvarManager->log("PreAsync in tut");
		//auto players = gameWrapper->GetGameEventAsServer().GetCars();
		//for (int i = 0; i < players.Count(); i++)
		//{
		//	auto player = players.Get(i);
		//	if (player.IsNull())
		//		continue;
			ControllerInput* ci = (ControllerInput*)params;
			if (*disableHandbrake)
			{
				ci->Handbrake = false;
			}

			if (*disableBoost) 
			{
				ci->ActivateBoost = false;
				ci->HoldingBoost = false;
			}

			if (*disableJump) 
			{
				ci->Jump = false;
				ci->Jumped = false;
			}

			if (*holdBoost)
			{
				ci->ActivateBoost = true;
				ci->HoldingBoost = true;
				//cvarManager->log("blocked boost");
			}

			ci->Steer = clip(ci->Steer, -abs(*limitSteer), abs(*limitSteer));
			ci->Throttle = clip(ci->Throttle, -abs(*limitThrottle), abs(*limitThrottle));
			ci->Yaw = clip(ci->Yaw, -abs(*limitYaw), abs(*limitYaw));
			ci->Pitch = clip(ci->Pitch, -abs(*limitPitch), abs(*limitPitch));
			ci->Roll = clip(ci->Roll, -abs(*limitRoll), abs(*limitRoll));
			//player.SetInput(ci);
			
		
	}
}

void MechanicalPlugin::OnFreeplayLoad(std::string eventName)
{
	if(*enabled)
		gameWrapper->HookEventWithCaller<CarWrapper>("Function TAGame.Car_TA.SetVehicleInput",
			bind(&MechanicalPlugin::OnPreAsync, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

void MechanicalPlugin::OnFreeplayDestroy(std::string eventName)
{
	gameWrapper->UnhookEvent("Function TAGame.RBActor_TA.PreAsyncTick");
}

void MechanicalPlugin::OnEnabledChanged(std::string oldValue, CVarWrapper cvar)
{
	if (cvar.getBoolValue() && gameWrapper->IsInTutorial())
	{
		gameWrapper->HookEventWithCaller<CarWrapper>("Function TAGame.Car_TA.SetVehicleInput",
			bind(&MechanicalPlugin::OnPreAsync, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	}
	else
	{
		gameWrapper->UnhookEvent("Function TAGame.RBActor_TA.PreAsyncTick");
	}
}