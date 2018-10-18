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
	BakkesMod::Plugin::BakkesModPlugin
{
private:
	string getSafeFileName(string initialName);
	std::unique_ptr<BakPy> _bakPy;
	int _executes = 0;
#ifdef TICK_BUILD
	std::shared_ptr<bool> enableTickable;
#endif
public:
	void reinit_python();
	
	virtual void onLoad();
	virtual void onUnload();
#ifdef TICK_BUILD
	virtual void on_tick(std::string ignoredParam);
#endif
};


