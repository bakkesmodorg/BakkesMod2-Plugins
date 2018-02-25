#pragma once
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

enum PlotStatus
{
	PlotStatus_STOPPED = 0,
	PlotStatus_RECORDING,
	PlotStatus_PLAYING
};
struct frame {
	float timestamp = .0;
	ControllerInput input = {0};
	Vector locationData = { 0 };
	Rotator rotationData = { 0 };
	Vector velocityData = { 0 };
};
struct recording {
	float starttime = .0f;
	float endtime = .0f;
	int framecount = .0f;
	std::shared_ptr<vector<std::shared_ptr<frame>>> frames;
};

class MacroPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	std::vector<string> playbackData;
	PlotStatus currentStatus = PlotStatus_STOPPED;
	float playbackStartTime = .0f;
	recording currentRecording;
	std::shared_ptr<frame> currentPlaybackFrame;
	int currentPlaybackIndex = 0;
public:
	virtual void onLoad();
	virtual void onUnload();
	void logPlaybackData(string filename);
	void OnMacroCommand(std::vector<std::string> params);
	void OnPreAsync(std::string funcName);
	void OnRecordTick();
	void OnPlaybackTick();
	static frame interp(std::shared_ptr<frame> f1, std::shared_ptr<frame> f2, float elaps);
};

