#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

class MechanicalPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	shared_ptr<float> limitSteer;
	shared_ptr<float> limitThrottle;
	shared_ptr<float> limitYaw;
	shared_ptr<float> limitPitch;
	shared_ptr<float> limitRoll;
	shared_ptr<bool> disableHandbrake;
	shared_ptr<bool> disableJump;
	shared_ptr<bool> disableBoost;
public:
	virtual void onLoad();
	virtual void onUnload();
	void OnPreAsync(std::string funcName);
};

