#pragma once
#include <memory>
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"

/*
Colors the prediction line can have
*/
struct LineColor
{
	unsigned char r, g, b, a; //rgba can be a value of 0-255
};

/*Predicted point in 3d space*/
struct PredictedPoint
{
	/*Location of the predicted ball*/
	Vector location;
	/*States whether it as its highest point or bounces*/
	bool isApex = false;
	Vector apexLocation = { 0,0,0 };
	Vector velocity;
	Vector angVel;
};

class BallPredictionPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	std::shared_ptr<bool> predictOn;
	std::shared_ptr<int> predictSteps;
	std::shared_ptr<float> predictStepSize;
	LineColor colors[2] = { {0, 255, 0, 240}, {75, 0, 130, 240} };
	PredictedPoint predictedPoints[1000];
public:
	BallPredictionPlugin();
	~BallPredictionPlugin();
	virtual void onLoad();
	virtual void onUnload();
	
	void Predict(std::string eventName);

	void OnFreeplayLoad(std::string eventName);
	void OnFreeplayDestroy(std::string eventName);
	void OnPredictOnValueChanged(std::string oldValue, CVarWrapper cvar);
	void Render(CanvasWrapper canvas);
};

