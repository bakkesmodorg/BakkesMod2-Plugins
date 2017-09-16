#pragma once
#pragma comment( lib, "BakkesMod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "jsonshot.h"

struct shot_data {
	float ball_location_x = 0;
	float ball_location_y = 0;
	float ball_location_z = 0;

	float ball_velocity_x = 0;
	float ball_velocity_y = 0;
	float ball_velocity_z = 0;

	float ball_torque_pitch = 0;
	float ball_torque_roll = 0;
	float ball_torque_yaw = 0;

	float player_location_x = 0;
	float player_location_y = 0;
	float player_location_z = 0;

	float player_velocity_x = 0;
	float player_velocity_y = 0;
	float player_velocity_z = 0;

	int player_rotation_pitch = 0;
	int player_rotation_yaw = 0;
	int player_rotation_roll = 0;

	Vector get_ball_start_location() {
		return Vector(ball_location_x, ball_location_y, ball_location_z);
	};

	Vector get_ball_start_velocity() {
		return Vector(ball_velocity_x, ball_velocity_y, ball_velocity_z);
	};

	Vector get_ball_start_rotation() {
		return Vector(ball_torque_pitch, ball_torque_roll, ball_torque_yaw);
	};

	Vector get_player_start_location() {
		return Vector(player_location_x, player_location_y, player_location_z);
	};

	Vector get_player_start_velocity() {
		return Vector(player_velocity_x, player_velocity_y, player_velocity_z);
	};

	Rotator get_player_start_rotation() {
		return Rotator(player_rotation_pitch, player_rotation_yaw, player_rotation_roll);
	};
};



struct shot_list {
	std::vector<shot_data> shots;
};

class TrainingPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	shot_data s_data;
	JsonShot currentShot;
	bool is_loaded = false;
public:
	virtual void onLoad();
	virtual void onUnload();
	bool should_mirror();
	Vector mirror_it(Vector v, bool mir);
	Rotator mirror_it(Rotator r, bool mir);
	void get_shot_data_from_console(shot_data* data);
};
