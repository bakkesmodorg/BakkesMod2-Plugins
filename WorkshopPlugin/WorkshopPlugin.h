#pragma once
#pragma comment( lib, "BakkesMod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"


class WorkshopPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	vector<string> shotList;
	string load_after_request;
	int currentIndex = 0;
public:
	virtual void onLoad();
	virtual void onUnload();

	void next_shot();
	string createReplaySnapshot();
};
