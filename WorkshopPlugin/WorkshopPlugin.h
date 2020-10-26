#pragma once
#pragma comment( lib, "BakkesMod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"


class WorkshopPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	std::vector<std::string> shotList;
	std::string load_after_request;
	int currentIndex = 0;
public:
	virtual void onLoad();
	virtual void onUnload();

	void next_shot();
	std::string createReplaySnapshot();
};
