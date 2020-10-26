#include "WorkshopPlugin.h"
#include "utils/io.h"
#include "utils/parser.h"
#include <string>
#include <algorithm>

#include "bakkesmod/wrappers/ReplayServerWrapper.h"
#include "bakkesmod/wrappers/GameObject/BallWrapper.h"
#include "bakkesmod/wrappers/GameObject/CarWrapper.h"

#include <Windows.h>
#include <stdio.h>

BAKKESMOD_PLUGIN(WorkshopPlugin, "Workshop plugin", "0.1", PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING | PLUGINTYPE_REPLAY)

static const std::string REPLAY_SHOT_DIRECTORY = "./bakkesmod/shots/replay/";

std::string getSafeFileName(std::string folder, std::string baseName) {
	int currentFile = 0;
	std::string fileName;
	do {
		fileName = baseName + "_" + std::to_string(currentFile) + ".json";
		currentFile++;
	} while (file_exists(folder + "//" + fileName));

	return fileName;
}

void WorkshopPlugin::next_shot()
{
	if (cvarManager->getCvar("workshop_shot_random").getBoolValue())
	{
		currentIndex = random(0, shotList.size() - 1);
	}
	else {
		currentIndex++;
		if (currentIndex >= shotList.size())
			currentIndex = 0;
	}
	cvarManager->executeCommand("workshop_shot_load " + shotList.at(currentIndex));
}



std::string WorkshopPlugin::createReplaySnapshot() {
	ReplayServerWrapper gew = gameWrapper->GetGameEventAsReplay();
	BallWrapper b = gew.GetBall();
	ActorWrapper aw = gew.GetViewTarget();
	//data needed: ball loc, ball vel, vt loc, vt rotation

	std::ifstream t("./bakkesmod/data/replay_template.json");
	std::string json_template((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	replace(json_template, "{{player_loc_x}}", std::to_string(aw.GetLocation().X));
	replace(json_template, "{{player_loc_y}}", std::to_string(aw.GetLocation().Y));
	replace(json_template, "{{player_loc_z}}", std::to_string(aw.GetLocation().Z));

	replace(json_template, "{{player_vel_x}}", std::to_string(aw.GetVelocity().X));
	replace(json_template, "{{player_vel_y}}", std::to_string(aw.GetVelocity().Y));
	replace(json_template, "{{player_vel_z}}", std::to_string(aw.GetVelocity().Z));

	replace(json_template, "{{player_rot_pitch}}", std::to_string(aw.GetRotation().Pitch));
	replace(json_template, "{{player_rot_roll}}", std::to_string(aw.GetRotation().Roll));
	replace(json_template, "{{player_rot_yaw}}", std::to_string(aw.GetRotation().Yaw));


	replace(json_template, "{{ball_loc_x}}", std::to_string(b.GetLocation().X));
	replace(json_template, "{{ball_loc_y}}", std::to_string(b.GetLocation().Y));
	replace(json_template, "{{ball_loc_z}}", std::to_string(b.GetLocation().Z));

	replace(json_template, "{{ball_vel_x}}", std::to_string(b.GetVelocity().X));
	replace(json_template, "{{ball_vel_y}}", std::to_string(b.GetVelocity().Y));
	replace(json_template, "{{ball_vel_z}}", std::to_string(b.GetVelocity().Z));


	replace(json_template, "{{ball_rot_pitch}}", std::to_string(b.GetRotation().Pitch));
	replace(json_template, "{{ball_rot_roll}}", std::to_string(b.GetRotation().Roll));
	replace(json_template, "{{ball_rot_yaw}}", std::to_string(b.GetRotation().Yaw));

	replace(json_template, "{{ball_torque_x}}", std::to_string(b.GetAngularVelocity().X));
	replace(json_template, "{{ball_torque_y}}", std::to_string(b.GetAngularVelocity().Y));
	replace(json_template, "{{ball_torque_z}}", std::to_string(b.GetAngularVelocity().Z));

	std::string fileName = getSafeFileName(REPLAY_SHOT_DIRECTORY, "replay");
	//replace(json_template, "{{name}}", fileName);
	std::ofstream outputFile(REPLAY_SHOT_DIRECTORY + fileName);
	outputFile << json_template;
	outputFile.close();
	return fileName;
}

void WorkshopPlugin::onLoad()
{
	cvarManager->registerNotifier("echome", [this](std::vector<std::string> params) {
		if (params.size() < 2)
			return;

		std::string echo_back = params.at(1);
		cvarManager->executeCommand("sendback you sent me \"" + echo_back + "\"");

	}, "Sends the given string back to the rcon client", PERMISSION_ALL);

	cvarManager->registerNotifier("workshop_shot_load", [this](std::vector<std::string> params) {
		if (params.size() < 2)
			return;

		std::string shot_id = params.at(1);
		if (file_exists("./bakkesmod/shots/cache/" + shot_id + ".json"))
		{
			cvarManager->executeCommand("shot_load \"cache/" + shot_id + "\"");
			cvarManager->executeCommand("shot_generate");
			cvarManager->executeCommand("shot_reset");
		}
		else
		{
			cvarManager->executeCommand("sendback requestshot \"" + shot_id + "\"");
			load_after_request = shot_id;
		}
	}, "Loads a shot from the BakkesMod workshop (deprecated)", PERMISSION_ALL);

	cvarManager->registerNotifier("workshop_playlist_load", [this](std::vector<std::string> params) {
		if (params.size() < 2)
			return;

		std::string playlist_id = params.at(1);
		cvarManager->executeCommand("sendback requestplaylist \"" + playlist_id + " \"");
	}, "Loads a playlist from the BakkesMod workshop. Usage: workshop_playlist_load playlistid (deprecated)", PERMISSION_ALL);
	cvarManager->registerNotifier("workshop_playlist_next", [this](std::vector<std::string> params) {
		if (!shotList.empty())
		{
			next_shot();
		}
	}, "Goes to into the next shot from the playlist loaded from the BakkesMod workshop (deprecated)", PERMISSION_ALL);
	cvarManager->registerNotifier("requestshot_ans", [this](std::vector<std::string> params) {
		if (params.size() < 3)
			return;
		std::string shot_id = params.at(1);
		std::string shot_content = params.at(2);
		std::replace(shot_content.begin(), shot_content.end(), '|', '"');
		std::ofstream myfile;
		myfile.open("./bakkesmod/shots/cache/" + shot_id + ".json");
		myfile << shot_content;
		myfile.close();
		if (!load_after_request.empty())
		{
			cvarManager->executeCommand("workshop_shot_load " + load_after_request);
			load_after_request = "";
		}
	}, "Answer from BakkesMod workshop (should only be used by external RCON clients, not users) (deprecated)", PERMISSION_ALL);
	cvarManager->registerNotifier("replay_snapshot", [this](std::vector<std::string> params) {
		if (!gameWrapper->IsInReplay()) {
			return;
		}
		createReplaySnapshot();
	}, "Takes a replay snapshot from the current replay (deprecated)", PERMISSION_REPLAY);
	cvarManager->registerNotifier("replay_snapshot_request", [this](std::vector<std::string> params) {
		if (!gameWrapper->IsInReplay()) {
			cvarManager->executeCommand("sendback \"echo You need to be watching a replay to use this.\"");
			return;
		}
		std::string snapshotName = createReplaySnapshot();
		std::ifstream t(REPLAY_SHOT_DIRECTORY + snapshotName);
		std::string shotFile((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());
		std::replace(shotFile.begin(), shotFile.end(), '"', '|');
		cvarManager->executeCommand("sendback \"replay_snapshot_request_ans " + shotFile + "\"");
	}, "Sends a replay snapshot from the current replay to the BakkesMod workshop (Should only be used by rcon clients) (deprecated)", PERMISSION_ALL); 
	//Permission all since we need to send data back to the client

	cvarManager->registerNotifier("requestplaylist_ans", [this](std::vector<std::string> params) {
		if (params.size() < 3)
			return;
		std::string playlist_id = params.at(1);
		std::string shots = params.at(2);
		shotList.empty();

		if (shots.size() == 0)
			return;
		split(shots, shotList, ',');
		currentIndex = -1;
		next_shot();
	}, "Answer from BakkesMod workshop about the current playlist (should only be used by external RCON clients, not users) (deprecated)", PERMISSION_ALL);
	//cons->registerNotifier("workshop_playlist_prev", workshop_notifier);//workshop_shot_random
	//cvarManager->registerCvar("workshop_playlist_random", "1"); 
	cvarManager->registerCvar("workshop_shot_random", "1");

}

void WorkshopPlugin::onUnload()
{
}