#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

class MechanicalPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	std::shared_ptr<bool> enabled;
	std::shared_ptr<float> limitSteer;
	std::shared_ptr<float> limitThrottle;
	std::shared_ptr<float> limitYaw;
	std::shared_ptr<float> limitPitch;
	std::shared_ptr<float> limitRoll;
	std::shared_ptr<bool> disableHandbrake;
	std::shared_ptr<bool> disableJump;
	std::shared_ptr<bool> disableBoost;
	std::shared_ptr<bool> holdBoost;
	std::shared_ptr<bool> holdRoll;
public:
	virtual void onLoad();
	virtual void onUnload();
	void OnPreAsync(CarWrapper cw, void * params, std::string funcName);


	void OnFreeplayLoad(std::string eventName);
	void OnFreeplayDestroy(std::string eventName);
	void OnEnabledChanged(std::string oldValue, CVarWrapper cvar);

};

