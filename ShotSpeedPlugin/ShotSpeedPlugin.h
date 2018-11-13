#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

struct Popup
{
	float startTime = 0.f;
	std::string text = "";
	Vector2 startLocation = { -1, -1 };
};

class ShotSpeedPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	std::vector<Popup> popups;
	virtual void onLoad();
	virtual void onUnload();
	void OnHitBallPre(CarWrapper cw, void* params, std::string eventName);
	void OnHitBall(std::string eventName);
	void Render(CanvasWrapper canvas);
};

