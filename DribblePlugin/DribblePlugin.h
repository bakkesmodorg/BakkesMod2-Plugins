#pragma once
#pragma once
#pragma comment( lib, "BakkesMod Rewrite.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

class DribblePlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	virtual void onLoad();
	virtual void onUnload();
};

