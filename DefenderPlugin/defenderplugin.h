#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

class DefenderPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	bool defenderEnabled = false;
	float lastTouchTime = .0f;
	int destinationGoal = 0;
	int touchTimeout = 6;
	int currentTouchChecks = 0;
public:
	virtual void onLoad();
	virtual void onUnload();
	void CheckForShot();
	float HandleShot();
	void ExecuteShot();
};

