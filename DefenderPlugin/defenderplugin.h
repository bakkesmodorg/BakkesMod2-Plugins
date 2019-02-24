#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

class DefenderPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	/*
	Whether defender plugin is currently active
	*/
	std::shared_ptr<bool> pluginActive = std::make_shared<bool>(false);

	/*
	Last time the ball was hit by player
	*/
	float lastTouchTime = .0f;

	/*
	The goal to aim on
	*/
	int destinationGoal = 0;

	float nextShotTimeout = .0f;
public:
	virtual void onLoad();
	virtual void onUnload();

	float GenerateNextTimeout();

	void CheckForShot();
	void HandleShot();


	void OnGoalScored(std::string eventName);
	void OnHitBall(std::string eventName);
	void OnDefenderStatusChanged(std::string oldValue, CVarWrapper newValue);
};

