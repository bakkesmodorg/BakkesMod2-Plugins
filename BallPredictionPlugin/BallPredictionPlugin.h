#pragma once
#include <memory>
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

struct LineColor
{
	unsigned char r, g, b, a; //rgba can be a value of 0-255
};

class BallPredictionPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	std::shared_ptr<bool> predictOn;
	std::shared_ptr<int> predictSteps;
	std::shared_ptr<float> predictStepSize;
	LineColor colors[2] = { {0, 0, 0, 255}, {255, 255, 255, 255} };
public:
	BallPredictionPlugin();
	~BallPredictionPlugin();
	virtual void onLoad();
	void Render(CanvasWrapper canvas);
	virtual void onUnload();
	
};

