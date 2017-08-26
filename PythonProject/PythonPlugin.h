#pragma once
#define BOOST_PYTHON_STATIC_LIB   
#include "boost/python/detail/wrap_python.hpp"
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

//#include "plugin/botplugin.h"

//Plugin context for python callbacks and timeouts and stuff
class BakPy {
public:
	std::shared_ptr<GameWrapper> gameWrapper;
	std::shared_ptr<CVarManagerWrapper> cvarManager;
	BakPy(std::shared_ptr<GameWrapper> gw, std::shared_ptr<CVarManagerWrapper> cw);

	void set_timeout(string methodname, float time);
};


class PythonPlugin : public
#ifdef _AI_BUILD
	BakkesMod::Plugin::BotPlugin
#else
	BakkesMod::Plugin::BakkesModPlugin
#endif
{
private:
	string getSafeFileName(string initialName);
	std::unique_ptr<BakPy> _bakPy;
	int _executes = 0;
public:
	void reinit_python();
	
	virtual void onLoad();
	virtual void onUnload();
#ifdef _AI_BUILD
	virtual void on_tick(ControllerInput *input, CarWrapper *localCar, BallWrapper *ball);
#endif
};


