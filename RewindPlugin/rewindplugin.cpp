#include "rewindplugin.h"
#include "utils/parser.h"
#include "utils/customrotator.h"

#include "bakkesmod\wrappers\tutorialwrapper.h"
#include "bakkesmod\wrappers\carcomponent\boostwrapper.h"


using namespace std::placeholders;


BAKKESMOD_PLUGIN(RewindPlugin, "Rewind plugin", "0.2", PLUGINTYPE_FREEPLAY)

float snapshot_interval = 0.030f; //time (s) between updates
int max_history = 500; //500*0.030s
bool recorderEnabled = true;
bool rewinderEnabled = false;
bool overwriting = false;

class GameState {
public:
	Vector ball_location; //ball position
	Vector car_location;
	Vector ball_velocity;
	Vector car_velocity; //car velocity
	CustomRotator ball_rotation;
	CustomRotator car_rotation;
	Vector ball_ang_velocity;
	Vector car_ang_velocity;
	float boost_amount; //boost
	float timestamp;

	GameState(TutorialWrapper tw, float ts) {
		BallWrapper b = tw.GetBall();
		CarWrapper c = tw.GetGameCar();

		ball_location = b.GetLocation();
		car_location = c.GetLocation();
		ball_velocity = b.GetVelocity();
		car_velocity = c.GetVelocity();
		ball_rotation = b.GetRotation();
		car_rotation = c.GetRotation();
		ball_ang_velocity = b.GetAngularVelocity();
		car_ang_velocity = c.GetAngularVelocity();

		boost_amount = c.GetBoost().IsNull() ? 0 : c.GetBoost().GetCurrentBoostAmount();
		timestamp = ts;
	}

	GameState() {
		ball_location = Vector(0, 0, 0);
		car_location = Vector(0, 0, 0);
		ball_velocity = Vector(0, 0, 0);
		car_velocity = Vector(0, 0, 0);
		ball_rotation = Rotator(0, 0, 0);
		car_rotation = Rotator(0, 0, 0);
		ball_ang_velocity = Vector(0, 0, 0);
		car_ang_velocity = Vector(0, 0, 0);
		boost_amount = 0;
		timestamp = 0;
	}

	//for rewinding, interpolate between two instants
	void interpolate(GameState lhs, GameState rhs, float elapsed) {
		float custom_elapsed = elapsed * 1000;
		float intval = snapshot_interval * 1000;
		Vector snap = Vector(intval);
		CustomRotator snapR = CustomRotator(Rotator(intval));
		ball_location = lhs.ball_location + (((rhs.ball_location - lhs.ball_location) / snap) * custom_elapsed);
		car_location = lhs.car_location + (((rhs.car_location - lhs.car_location) / snap) * custom_elapsed);

		ball_velocity = lhs.ball_velocity + (((rhs.ball_velocity - lhs.ball_velocity) / snap) * custom_elapsed);
		car_velocity = lhs.car_velocity + (((rhs.car_velocity - lhs.car_velocity) / snap) * custom_elapsed);

		//ball_rotation = lhs.ball_rotation + (((rhs.ball_rotation - lhs.ball_rotation) / snapR) * custom_elapsed);

		CustomRotator brot1 = (lhs.ball_rotation.diffTo(rhs.ball_rotation));
		CustomRotator brot2 = brot1 / snapR;
		CustomRotator brot3 = brot2*CustomRotator(custom_elapsed);
		ball_rotation = lhs.ball_rotation + brot3;// (((rhs.car_rotation - lhs.car_rotation) / snapR) * custom_elapsed);



		float difd = (rhs.car_rotation.Pitch._value - lhs.car_rotation.Pitch._value);

		CustomRotator rot1 = (lhs.car_rotation.diffTo(rhs.car_rotation));
		CustomRotator rot2 = rot1 / snapR;
		CustomRotator rot3 = rot2*CustomRotator(custom_elapsed);
		car_rotation = lhs.car_rotation + rot3;// (((rhs.car_rotation - lhs.car_rotation) / snapR) * custom_elapsed);

		ball_ang_velocity = lhs.ball_ang_velocity + (((rhs.ball_ang_velocity - lhs.ball_ang_velocity) / snap) * custom_elapsed);
		car_ang_velocity = lhs.car_ang_velocity + (((rhs.car_ang_velocity - lhs.car_ang_velocity) / snap) * custom_elapsed);
		 
		boost_amount = lhs.boost_amount + (((rhs.boost_amount - lhs.boost_amount) / intval)*custom_elapsed);
	}

	void apply(TutorialWrapper tw) {
		BallWrapper b = tw.GetBall();
		CarWrapper c = tw.GetGameCar();
		b.SetLocation(ball_location);
		c.SetLocation(car_location);
		b.SetVelocity(ball_velocity);
		c.SetVelocity(car_velocity);
		b.SetRotation(ball_rotation.ToRotator());
		c.SetRotation(car_rotation.ToRotator());

		b.SetAngularVelocity(ball_ang_velocity);
		c.SetAngularVelocity(car_ang_velocity);

		c.GetBoost().SetBoostAmount(boost_amount);
	}
};

std::vector<GameState> history;
GameState favorite = GameState();
GameState overwrite = GameState();
int current = 0;
float lastPlaybackTime = 0;
float tickTime = 0;

void RewindPlugin::onLoad()
{
	//Patch 1.39, psyonix removed prephysicssteps does not get called anymore
	gameWrapper->HookEvent("Function PlayerController_TA.Driving.PlayerMove", bind(&RewindPlugin::OnPreAsync, this, _1)); //Function TAGame.PlayerController_TA.PrePhysicsStep

	cvarManager->registerCvar("sv_rewind_button", "XboxTypeS_LeftThumbStick", "Button for rewind controls").addOnValueChanged([this](std::string old, CVarWrapper now) {
		rewindKey = this->gameWrapper->GetFNameIndexByString(now.getStringValue());
		cvarManager->log("Rewind button is now " + to_string(rewindKey));
	});
	cvarManager->getCvar("sv_rewind_button").notify();
	recorderEnabled = true;

	cvarManager->registerNotifier("sv_rewind_play", [&cvarManager = this->cvarManager, &gameWrapper = this->gameWrapper](vector<string> command) {
		if (!gameWrapper->IsInTutorial())
			return;
		if (history.size() == 0) {
			cvarManager->log("No gamestates recorded");
			return;
		}
		rewinderEnabled = false;
		recorderEnabled = true;
		favorite = history.back();
	});
	cvarManager->registerNotifier("sv_rewind_favorite", [&cvarManager = this->cvarManager, &gameWrapper = this->gameWrapper](vector<string> command) {
		if (!gameWrapper->IsInTutorial())
			return;
		overwriting = true;
		overwrite = favorite;
		TutorialWrapper tw = gameWrapper->GetGameEventAsTutorial();
		tw.GetGameCar().Stop();
		overwrite.apply(tw);
	});
	//cvarManager->registerNotifier("sv_rewind_on", [&cvarManager = this->cvarManager, &gameWrapper = this->gameWrapper](vector<string> command) {
	//	if (!gameWrapper->IsInTutorial())
	//		return;
	//	recorderEnabled = true;
	//});
	/*cvarManager->registerNotifier("sv_rewind_rewind", [&cvarManager = this->cvarManager, &gameWrapper = this->gameWrapper](vector<string> command) {
		if (!gameWrapper->IsInTutorial())
			return;
		rewinderEnabled = true;
		recorderEnabled = false;
		lastPlaybackTime = gameWrapper->GetGameEventAsTutorial().GetSecondsElapsed();
	});
	
	cvarManager->registerNotifier("sv_rewind_favorite", [&cvarManager = this->cvarManager, &gameWrapper = this->gameWrapper](vector<string> command) {
		if (!gameWrapper->IsInTutorial())
			return;
		overwriting = true;
		overwrite = favorite;
		TutorialWrapper tw = gameWrapper->GetGameEventAsTutorial();
		tw.GetGameCar().Stop();
		overwrite.apply(tw);
	});*/
}

void RewindPlugin::onUnload()
{

}

float rewinderSpeed = .0f;
float lastTick = .0f;
float snapshotDiff = .0f;
float snapshotElapsed = .0f;
int rewindFunc = 0;
void RewindPlugin::OnPreAsync(std::string funcName)
{
	if (gameWrapper->IsInTutorial())
	{
		TutorialWrapper tw = gameWrapper->GetGameEventAsTutorial();
		
		//cvarManager->log("Pressed: " + to_string(gameWrapper->IsKeyPressed(idx)));
		if (gameWrapper->IsKeyPressed(rewindKey))
		{
			auto players = gameWrapper->GetGameEventAsServer().GetPlayers();
			if (players.Count() > 0) 
			{
				ControllerInput ci = players.Get(0).GetInput();
				//cvarManager->log("Steer: " + to_string(ci.Steer));
				if (ci.Steer < -.2f) {
					rewinderEnabled = true;
					rewinderSpeed = ci.Steer;
					rewindFunc = 2;
				}
				else {
					rewinderEnabled = false;
					recorderEnabled = true;
					if(rewindFunc != 2) //if player has rewinded, he has not tapped
						rewindFunc = 1;
				}
			}
				
		}
		else {
			rewinderEnabled = false;
			recorderEnabled = true;
			if (rewindFunc == 1)
			{
				cvarManager->executeCommand("sv_rewind_favorite", false);
			}
			rewindFunc = 0;
		}
		if (rewinderEnabled) {
			float currentTimeInMs = tw.GetSecondsElapsed();
			float tickDiff = currentTimeInMs - lastTick;
			if (abs(tickDiff) > .1) {
				lastTick = currentTimeInMs;
				tickDiff = .01f;
			}
			BallWrapper b = tw.GetBall();
			if (b.IsNull())
			{
				return;
			}
			CarWrapper c = tw.GetGameCar();
			if (c.IsNull())
			{
				return;
			}
			if (current <= 2) { //if no more history
				cvarManager->executeCommand("sv_rewind_play", false);
			}
			else {

				float deltaElapsed = tickDiff * abs(rewinderSpeed); //full left = 1 second/second
				snapshotElapsed += deltaElapsed;

				while (snapshotElapsed > snapshotDiff && history.size() > 2) {
					history.pop_back();
					current--;
					snapshotElapsed -= snapshotDiff;
					GameState lastGameState = history.at(history.size() - 2);
					GameState currentState = history.back();
					snapshotDiff = currentState.timestamp - lastGameState.timestamp;
					if (snapshotDiff > snapshot_interval) { //If user already rewinded once timestamps are wonky at those two points
						snapshotDiff = snapshot_interval;
						(history.at(history.size() - 2)).timestamp = history.back().timestamp - snapshot_interval;
					}
				}

				//cons->log("Tickdif: " + to_string(tickDiff) + ", ");

				GameState lastGameState = history.at(history.size() - 2);
				GameState currentState = history.back();
				overwrite.interpolate(currentState, lastGameState, snapshotElapsed);
				favorite = overwrite;
				
			}


			overwrite.apply(tw);
			overwriting = false;
			lastTick = currentTimeInMs;
		}
		else if (recorderEnabled)
		{
			record();
		}
		
	}
}

float lastRecordTime = .0f;
void RewindPlugin::record()
{
	if (!gameWrapper->IsInTutorial())
		return;
	TutorialWrapper tw = gameWrapper->GetGameEventAsTutorial();

	if (abs(tw.GetSecondsElapsed() - lastRecordTime) < snapshot_interval)
		return;
	BallWrapper b = tw.GetBall();
	CarWrapper c = tw.GetGameCar();
	if (b.IsNull() || c.IsNull())
		return;

	if (history.size() >= max_history)
	{
		history.erase(history.begin());
	}
	else {
		current++;
	}
	
	history.push_back(GameState(tw, tw.GetSecondsElapsed()));
	lastRecordTime = tw.GetSecondsElapsed();
}

