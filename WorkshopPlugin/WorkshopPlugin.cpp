#include "WorkshopPlugin.h"
#include "utils/io.h"
#include "utils/parser.h"
#include <string>
#include <algorithm>

#include <Windows.h>
#include <stdio.h>

BAKKESMOD_PLUGIN(WorkshopPlugin, "Workshop plugin", "0.1", PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING | PLUGINTYPE_REPLAY)

static const string REPLAY_SHOT_DIRECTORY = "./bakkesmod/shots/replay/";

std::string getSafeFileName(std::string folder, std::string baseName) {
	int currentFile = 0;
	string fileName;
	do {
		fileName = baseName + "_" + to_string(currentFile) + ".json";
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



string WorkshopPlugin::createReplaySnapshot() {
	ReplayWrapper gew = gameWrapper->GetGameEventAsReplay();
	BallWrapper b = gew.GetBall();
	ActorWrapper aw = gew.GetViewTarget();
	//data needed: ball loc, ball vel, vt loc, vt rotation

	std::ifstream t("./bakkesmod/data/replay_template.json");
	std::string json_template((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	replace(json_template, "{{player_loc_x}}", to_string(aw.GetLocation().X));
	replace(json_template, "{{player_loc_y}}", to_string(aw.GetLocation().Y));
	replace(json_template, "{{player_loc_z}}", to_string(aw.GetLocation().Z));

	replace(json_template, "{{player_rot_pitch}}", to_string(aw.GetRotation().Pitch));
	replace(json_template, "{{player_rot_roll}}", to_string(aw.GetRotation().Roll));
	replace(json_template, "{{player_rot_yaw}}", to_string(aw.GetRotation().Yaw));


	replace(json_template, "{{ball_loc_x}}", to_string(b.GetLocation().X));
	replace(json_template, "{{ball_loc_y}}", to_string(b.GetLocation().Y));
	replace(json_template, "{{ball_loc_z}}", to_string(b.GetLocation().Z));

	replace(json_template, "{{ball_vel_x}}", to_string(b.GetVelocity().X));
	replace(json_template, "{{ball_vel_y}}", to_string(b.GetVelocity().Y));
	replace(json_template, "{{ball_vel_z}}", to_string(b.GetVelocity().Z));



	string fileName = getSafeFileName(REPLAY_SHOT_DIRECTORY, "replay");
	//replace(json_template, "{{name}}", fileName);
	std::ofstream outputFile(REPLAY_SHOT_DIRECTORY + fileName);
	outputFile << json_template;
	outputFile.close();
	return fileName;
}

void WorkshopPlugin::onLoad()
{
	cvarManager->registerNotifier("workshop_shot_load", [this](vector<string> params) {
		if (params.size() < 2)
			return;

		string shot_id = params.at(1);
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
	});

	cvarManager->registerNotifier("workshop_playlist_load", [this](vector<string> params) {
		if (params.size() < 2)
			return;

		string playlist_id = params.at(1);
		cvarManager->executeCommand("sendback requestplaylist \"" + playlist_id + " \"");
	});
	cvarManager->registerNotifier("workshop_playlist_next", [this](vector<string> params) {
		if (!shotList.empty())
		{
			next_shot();
		}
	});
	cvarManager->registerNotifier("requestshot_ans", [this](vector<string> params) {
		if (params.size() < 3)
			return;
		string shot_id = params.at(1);
		string shot_content = params.at(2);
		std::replace(shot_content.begin(), shot_content.end(), '|', '"');
		ofstream myfile;
		myfile.open("./bakkesmod/shots/cache/" + shot_id + ".json");
		myfile << shot_content;
		myfile.close();
		if (!load_after_request.empty())
		{
			cvarManager->executeCommand("workshop_shot_load " + load_after_request);
			load_after_request = "";
		}
	});
	cvarManager->registerNotifier("replay_snapshot", [this](vector<string> params) {
		if (!gameWrapper->IsInReplay()) {
			return;
		}
		createReplaySnapshot();
	});
	cvarManager->registerNotifier("replay_snapshot_request", [this](vector<string> params) {
		if (!gameWrapper->IsInReplay()) {
			cvarManager->executeCommand("sendback \"echo You need to be watching a replay to use this.\"");
			return;
		}
		string snapshotName = createReplaySnapshot();
		std::ifstream t(REPLAY_SHOT_DIRECTORY + snapshotName);
		std::string shotFile((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());
		std::replace(shotFile.begin(), shotFile.end(), '"', '|');
		cvarManager->executeCommand("sendback \"replay_snapshot_request_ans " + shotFile + "\"");
	});
	cvarManager->registerNotifier("requestplaylist_ans", [this](vector<string> params) {
		if (params.size() < 3)
			return;
		string playlist_id = params.at(1);
		string shots = params.at(2);
		shotList.empty();

		if (shots.size() == 0)
			return;
		split(shots, shotList, ',');
		currentIndex = -1;
		next_shot();
	});
	//cons->registerNotifier("workshop_playlist_prev", workshop_notifier);//workshop_shot_random
	//cvarManager->registerCvar("workshop_playlist_random", "1"); 
	cvarManager->registerCvar("workshop_shot_random", "1");

}

void WorkshopPlugin::onUnload()
{
}