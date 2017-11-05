#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

//Rewindplugin: initial version created by CJ (
class RewindPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	int rewindKey = 0;
	//std::shared_ptr<std::string> _rewindKey = make_shared<string>("XboxTypeS_X");
public:
	virtual void onLoad();
	virtual void onUnload();
	void OnPreAsync(std::string funcName);
	void record();
};
