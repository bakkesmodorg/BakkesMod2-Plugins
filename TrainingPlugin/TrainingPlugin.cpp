#include "trainingplugin.h"
#include "cvareval.h"
#include <math.h>


BAKKESMOD_PLUGIN(TrainingPlugin, "Training plugin", "0.1.1", PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING)

void TrainingPlugin::get_shot_data_from_console(shot_data* data) {
	ball b;
	player p;
	speed s;

	data->ball_location_x = cvarManager->getCvar("shot_initial_ball_location_x").getFloatValue();
	data->ball_location_y = cvarManager->getCvar("shot_initial_ball_location_y").getFloatValue();
	data->ball_location_z = cvarManager->getCvar("shot_initial_ball_location_z").getFloatValue();

	b.location.x = to_string(data->ball_location_x);
	b.location.y = to_string(data->ball_location_y);
	b.location.z = to_string(data->ball_location_z);
	CVarWrapper velocityRandomization = cvarManager->getCvar("shot_randomization");
	data->ball_velocity_x = cvarManager->getCvar("shot_initial_ball_velocity_x").getFloatValue() + velocityRandomization.getFloatValue();
	data->ball_velocity_y = cvarManager->getCvar("shot_initial_ball_velocity_y").getFloatValue() + velocityRandomization.getFloatValue();
	data->ball_velocity_z = cvarManager->getCvar("shot_initial_ball_velocity_z").getFloatValue() + velocityRandomization.getFloatValue();

	b.velocity.x = to_string(data->ball_velocity_x);
	b.velocity.y = to_string(data->ball_velocity_y);
	b.velocity.z = to_string(data->ball_velocity_z);

	data->ball_torque_pitch = cvarManager->getCvar("shot_initial_ball_torque_pitch").getFloatValue();
	data->ball_torque_roll = cvarManager->getCvar("shot_initial_ball_torque_roll").getFloatValue();
	data->ball_torque_yaw = cvarManager->getCvar("shot_initial_ball_torque_yaw").getFloatValue();

	data->player_location_x = get_safe_float(parse(cvarManager->getCvar("shot_initial_player_location_x").getStringValue(), p, b, s));
	data->player_location_y = get_safe_float(parse(cvarManager->getCvar("shot_initial_player_location_y").getStringValue(), p, b, s));
	data->player_location_z = get_safe_float(parse(cvarManager->getCvar("shot_initial_player_location_z").getStringValue(), p, b, s));



	data->player_velocity_x = get_safe_float(parse(cvarManager->getCvar("shot_initial_player_velocity_x").getStringValue(), p, b, s));
	data->player_velocity_y = get_safe_float(parse(cvarManager->getCvar("shot_initial_player_velocity_y").getStringValue(), p, b, s));
	data->player_velocity_z = get_safe_float(parse(cvarManager->getCvar("shot_initial_player_velocity_z").getStringValue(), p, b, s));

	data->player_rotation_pitch = get_safe_int(parse(cvarManager->getCvar("shot_initial_player_rotation_pitch").getStringValue(), p, b, s));
	data->player_rotation_yaw = get_safe_int(parse(cvarManager->getCvar("shot_initial_player_rotation_yaw").getStringValue(), p, b, s));
	data->player_rotation_roll = get_safe_int(parse(cvarManager->getCvar("shot_initial_player_rotation_roll").getStringValue(), p, b, s));
}

Vector TrainingPlugin::mirror_it(Vector v, bool mir) {
	if (mir) {
		v.X = -v.X;
		//v.Y = -v.Y;
	}
	return v;
}

Rotator TrainingPlugin::mirror_it(Rotator r, bool mir) {
	if (mir) {
		if (r.Yaw > 0) {
			if (r.Yaw > 16383) {
				r.Yaw = 16383 - (r.Yaw - 16383);
			}
			else {
				r.Yaw = 16383 + (16383 - r.Yaw);

			}
		}
		else {
			if (r.Yaw > -16383) {
				r.Yaw = -16383 - (16383 - abs(r.Yaw));
			}
			else {
				r.Yaw = -16383 + (abs(r.Yaw) - 16383);
			}
		}

		//r.Yaw += (r.Yaw <= 0) ? 32766 : -32767;
	}
	return r;
}

bool TrainingPlugin::should_mirror() {
	return cvarManager->getCvar("shot_mirror").getBoolValue();
}

void TrainingPlugin::onLoad()
{
	cvarManager->registerCvar("shot_mirror", "0");
	cvarManager->registerCvar("shot_initial_ball_location_x", "0");
	cvarManager->registerCvar("shot_initial_ball_location_y", "0");
	cvarManager->registerCvar("shot_initial_ball_location_z", "0");

	cvarManager->registerCvar("shot_initial_ball_velocity_x", "0");
	cvarManager->registerCvar("shot_initial_ball_velocity_y", "0");
	cvarManager->registerCvar("shot_initial_ball_velocity_z", "0");

	cvarManager->registerCvar("shot_initial_ball_torque_pitch", "0");
	cvarManager->registerCvar("shot_initial_ball_torque_roll", "0");
	cvarManager->registerCvar("shot_initial_ball_torque_yaw", "0");

	cvarManager->registerCvar("shot_initial_player_location_x", "0");
	cvarManager->registerCvar("shot_initial_player_location_y", "0");
	cvarManager->registerCvar("shot_initial_player_location_z", "0");

	cvarManager->registerCvar("shot_initial_player_velocity_x", "0");
	cvarManager->registerCvar("shot_initial_player_velocity_y", "0");
	cvarManager->registerCvar("shot_initial_player_velocity_z", "0");

	cvarManager->registerCvar("shot_initial_player_rotation_pitch", "0");
	cvarManager->registerCvar("shot_initial_player_rotation_yaw", "0");
	cvarManager->registerCvar("shot_initial_player_rotation_roll", "0");

	cvarManager->registerCvar("shot_randomization", "0"); //Shot randomization to velocity, in unreal units

	cvarManager->registerCvar("shot_countdowntime", "1");
	cvarManager->registerCvar("shot_waitbeforeshot", "0.5");

	cvarManager->registerNotifier("shot_reset", [this](vector<string> commands) {

		if (!gameWrapper->IsInTutorial() || !is_loaded)
			return;
		get_shot_data_from_console(&s_data);
		TutorialWrapper tw = gameWrapper->GetGameEventAsTutorial();
		bool mirror = should_mirror();
		if (tw.IsInFreePlay())
		{
			float waitTime = cvarManager->getCvar("shot_waitbeforeshot").getFloatValue();

			BallWrapper ball = tw.GetBall();
			ball.Stop();
			ball.SetLocation(mirror_it(s_data.get_ball_start_location(), mirror));
			ball.Stop();
			CarWrapper car = tw.GetGameCar();
			car.Stop();
			car.SetLocation(mirror_it(s_data.get_player_start_location(), mirror));
			car.SetCarRotation(mirror_it(s_data.get_player_start_rotation(), mirror));
			car.SetVelocity(mirror_it(s_data.get_player_start_velocity(), mirror));
			if (!currentShot.getShot().options.script.empty()) {
				cvarManager->executeCommand(currentShot.getShot().options.script);
			}
			//gw->SetTimeout([&](GameWrapper* gw) {
			//	if (!gw->IsInTutorial())
			//		return;
			//	TutorialWrapper tw = gw->GetGameEventAsTutorial();

			//	if (tw.IsInFreePlay()) {
			//		bool mirror = should_mirror();
			//		CarWrapper car = tw.GetGameCar();
			//		get_shot_data_from_console(&s_data);
			//		
			//	}
			//}, 50.0f);

			gameWrapper->SetTimeout([&,&_cvarManager = cvarManager](GameWrapper* gw) {
				if (!gw->IsInTutorial())
					return;
				TutorialWrapper tw = gw->GetGameEventAsTutorial();

				if (tw.IsInFreePlay())
				{

					bool mirror = should_mirror();
					BallWrapper ball = tw.GetBall();
					currentShot.setVelocity(gw, _cvarManager.get(), mirror_it(ball.GetLocation(), mirror));
					get_shot_data_from_console(&s_data);
					ball.SetVelocity(mirror_it(s_data.get_ball_start_velocity(), mirror));
					ball.SetTorque(mirror_it(s_data.get_ball_start_rotation(), mirror));
				}


			}, max(0.0f, waitTime));
		}
		else// if (tw.IsTraining(STRIKER) || tw.IsTraining(GOALIE) || tw.IsTraining(AERIAL))
		{
			tw.SetCountdownTime(cvarManager->getCvar("shot_countdowntime").getFloatValue());
			tw.SetBallSpawnLocation(mirror_it(s_data.get_ball_start_location(), mirror));
			tw.SetBallStartVelocity(mirror_it(s_data.get_ball_start_velocity(), mirror));
			tw.SetCarSpawnLocation(mirror_it(s_data.get_player_start_location(), mirror));
			tw.SetCarSpawnRotation(mirror_it(s_data.get_player_start_rotation(), mirror));
		}
	});
	cvarManager->registerNotifier("shot_load", [this](vector<string> commands) {
		if (!gameWrapper->IsInTutorial() || commands.size() == 1)
			return;
		string file = commands.at(1);
		currentShot = JsonShot(file);
		currentShot.init(gameWrapper.get(), cvarManager.get());
		is_loaded = true;
	});
	cvarManager->registerNotifier("shot_generate", [this](vector<string> commands) {
		if (!gameWrapper->IsInTutorial() || !is_loaded)
			return;
		currentShot.set(gameWrapper.get(), cvarManager.get());
	});
}

void TrainingPlugin::onUnload()
{
}
