#pragma once
#pragma comment( lib, "BakkesMod Rewrite.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

class RedirectPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
public:
	virtual void onLoad();
	virtual void onUnload();
};
