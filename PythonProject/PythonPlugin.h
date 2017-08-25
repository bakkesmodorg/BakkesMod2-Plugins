#pragma once
#define BOOST_PYTHON_STATIC_LIB   
#include "boost/python/detail/wrap_python.hpp"
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

//#include "plugin/botplugin.h"

class PythonPlugin : public
#ifdef _AI_BUILD
	BakkesMod::Plugin::BotPlugin
#else
	BakkesMod::Plugin::BakkesModPlugin
#endif
{
private:
	string getSafeFileName(string initialName);
public:
	void reinit_python();
	void set_timeout(string methodname, long long time);
	virtual void onLoad();
	virtual void onUnload();
#ifdef _AI_BUILD
	virtual void on_tick(ControllerInput *input, CarWrapper *localCar, BallWrapper *ball);
#endif
};


