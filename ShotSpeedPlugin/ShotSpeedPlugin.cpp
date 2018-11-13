#include "ShotSpeedPlugin.h"
#include "bakkesmod\wrappers\includes.h"
#include "utils/parser.h"
BAKKESMOD_PLUGIN(ShotSpeedPlugin, "Shotspeed plugin", "0.1", PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING)
std::ofstream interaction_file;


void ShotSpeedPlugin::onLoad()
{
	gameWrapper->HookEventWithCaller<CarWrapper>("Function TAGame.Car_TA.OnRigidBodyCollision", std::bind(&ShotSpeedPlugin::OnHitBallPre, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	gameWrapper->HookEventPost("Function TAGame.Car_TA.EventHitBall", std::bind(&ShotSpeedPlugin::OnHitBall, this, std::placeholders::_1));
	gameWrapper->RegisterDrawable(std::bind(&ShotSpeedPlugin::Render, this, std::placeholders::_1));
	interaction_file.open("./bakkesmod/interactions.csv", std::ios_base::app);
}

void ShotSpeedPlugin::onUnload()
{
}
struct CollisionStruct
{
	void*												Actor;                                            		// 0x0000 (0x0004) [0x0000000000000000]              
	void*												Component;                                        		// 0x0004 (0x0004) [0x0000000004080008]              ( CPF_ExportObject | CPF_Component | CPF_EditInline )
	void*												PhysMat;                                          		// 0x0008 (0x0004) [0x0000000000000000]              
	struct Vector										Velocity;                                         		// 0x000C (0x000C) [0x0000000000000000]              
	struct Vector										OtherVelocity;                                    		// 0x0018 (0x000C) [0x0000000000000000]              
	struct Vector										Location;                                         		// 0x0024 (0x000C) [0x0000000000000000]              
	struct Vector										Normal;                                           		// 0x0030 (0x000C) [0x0000000000000000]              
	struct Vector										NormalForce;                                      		// 0x003C (0x000C) [0x0000000000000000]              
	struct Vector										FrictionForce;                                    		// 0x0048 (0x000C) [0x0000000000000000]              
	struct Vector										NormalVelocity;                                   		// 0x0054 (0x000C) [0x0000000000000000]              
	struct Vector										FrictionVelocity;                                 		// 0x0060 (0x000C) [0x0000000000000000]              
	int													NumCollisions;                                    		// 0x006C (0x0004) [0x0000000000000000]              
	int													NumContacts;                                      		// 0x0070 (0x0004) [0x0000000000000000]              
};

struct RelevantData
{
	float time = 0;
	Vector OldLocation;
	Vector HitDir;
	Quat forwardDir;
	float relSpeed;
};
RelevantData latest;
void ShotSpeedPlugin::OnHitBallPre(CarWrapper cw, void * params, std::string eventName)
{
	if (!gameWrapper->IsInGame())
		return;
	if (abs(gameWrapper->GetGameEventAsServer().GetSecondsElapsed() - latest.time) < 0.1f)
		return;
	CollisionStruct* collision = (CollisionStruct*)params;
	BallWrapper bw = BallWrapper((uintptr_t)collision->Actor);
	RBState ballRB = bw.GetOldRBState();
	RBState carRB = cw.GetOldRBState();
	Vector rbDiff = { ballRB.LinearVelocity.X - carRB.LinearVelocity.X, ballRB.LinearVelocity.Y - carRB.LinearVelocity.Y, ballRB.LinearVelocity.Z - carRB.LinearVelocity.Z };
	Vector hitDir = { ballRB.Location.X - carRB.Location.X, ballRB.Location.Y - carRB.Location.Y, ballRB.Location.Z - carRB.Location.Z };
	hitDir.normalize();
	float relSpeed = rbDiff.magnitude(); // sqrt(pow(rbDiff.X, 2) + pow(rbDiff.Y, 2) + pow(rbDiff.Z, 2));
	Quat forwardDir = carRB.Quaternion;
	latest = { gameWrapper->GetGameEventAsServer().GetSecondsElapsed(), ballRB.Location, hitDir, forwardDir, relSpeed };
	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		
		auto tutorial = gameWrapper->GetGameEventAsServer();
		auto ball = tutorial.GetBall();
		RBState out = ball.GetRBState();
		Vector locationDiff = { out.Location.X - latest.OldLocation.X, out.Location.Y - latest.OldLocation.Y, out.Location.Z - latest.OldLocation.Z };
		interaction_file << latest.HitDir.X << "," << latest.HitDir.Y << "," << latest.HitDir.Z << ",";
		interaction_file << latest.forwardDir.X << "," << latest.forwardDir.Y << "," << latest.forwardDir.Z  << "," << latest.forwardDir.W << ",";
		interaction_file << latest.relSpeed << ",";
		interaction_file << out.LinearVelocity.X << "," << out.LinearVelocity.Y << "," << out.LinearVelocity.Z << ",";
		interaction_file << out.AngularVelocity.X << "," << out.AngularVelocity.Y << "," << out.AngularVelocity.Z << ",";
		interaction_file << locationDiff.X << "," << locationDiff.Y << "," << locationDiff.Z << "" << std::endl;
	}, 0.001f);
}

void ShotSpeedPlugin::OnHitBall(std::string eventName)
{
	if (!gameWrapper->IsInGame())
		return;
	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		if (!gameWrapper->IsInGame())
			return;
		auto tutorial = gameWrapper->GetGameEventAsServer();
		Vector ballLoc = tutorial.GetGameBalls().Get(0).GetRBState().LinearVelocity;
		float speed = sqrt(pow(ballLoc.X, 2) + pow(ballLoc.Y, 2) + pow(ballLoc.Z, 2));

		float kmh = (speed * 60 * 60) / 100000;
		string speedString;
		speedString = to_string_with_precision(kmh, 2) + " km/h";
		Popup p = { tutorial.GetSecondsElapsed(), speedString };
		popups.push_back(p);
	}, 0.2f);
}

void ShotSpeedPlugin::Render(CanvasWrapper canvas)
{
	if (!gameWrapper->IsInGame() || popups.empty())
		return;
	auto tutorial = gameWrapper->GetGameEventAsServer();
	if (tutorial.GetCars().Count() == 0)
		return;

	auto currentTime = tutorial.GetSecondsElapsed();
	auto carLoc2D = canvas.Project(tutorial.GetCars().Get(0).GetLocation());
	for (auto it = popups.begin(); it != popups.end(); )
	{
		if (it->startLocation.X < 0)
		{
			it->startLocation = carLoc2D;
		}
		float timeDiff = currentTime - it->startTime;
		if (timeDiff > 2.5)
		{
			it = popups.erase(it);
			continue;
		}
		Vector2 drawLoc = { it->startLocation.X - 140, (it->startLocation.Y - 320) - (timeDiff * 350) };
		canvas.SetPosition(drawLoc);
		canvas.SetColor(255, 255, 255, 255);
		canvas.DrawString(it->text, 3, 3);
		//cvarManager->log("x: " + to_string(carLoc2D.X) + ", y: " + to_string(carLoc2D.Y));
		it++;
	}
}

