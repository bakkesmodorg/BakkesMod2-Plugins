#include "MacroPlugin.h"
#include "bakkesmod/wrappers/GameEvent/ServerWrapper.h"
#include "bakkesmod/wrappers/GameObject/CarWrapper.h"
#include "bakkesmod/wrappers/ArrayWrapper.h"

#include <fstream>
#include "utils/io.h"
BAKKESMOD_PLUGIN(MacroPlugin, "Macro plugin", "0.2", PLUGINTYPE_FREEPLAY)




void MacroPlugin::onLoad()
{
	cvarManager->registerNotifier("plot_start", std::bind(&MacroPlugin::OnMacroCommand, this, std::placeholders::_1), "Starts/stops recording of macro", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("plot_playback", std::bind(&MacroPlugin::OnMacroCommand, this, std::placeholders::_1), "Plays back last recorded macro", PERMISSION_FREEPLAY);
	cvarManager->registerNotifier("plot_save", std::bind(&MacroPlugin::OnMacroCommand, this, std::placeholders::_1), "Saves recorded macro to given file (requires /recordings/ folder). Usage: plot_save savename", PERMISSION_ALL);
	cvarManager->registerNotifier("plot_load", std::bind(&MacroPlugin::OnMacroCommand, this, std::placeholders::_1), "Loads recorded macro from given file. Usage: plot_load savename", PERMISSION_ALL);
	//gameWrapper->HookEvent("Function TAGame.Car_TA.SetVehicleInput", bind(&MacroPlugin::OnPreAsync, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.Car_TA.SetVehicleInput", bind(&MacroPlugin::OnPreAsync, this, std::placeholders::_1));
	//cvarManager->log("Trying to hook");
	//gameWrapper->HookEventPost("Test", bind(&MacroPlugin::OnPreAsync, this, std::placeholders::_1), 1);
	//gameWrapper->HookEvent("Test", bind(&MacroPlugin::OnPreAsync, this, std::placeholders::_1));
	//gameWrapper->HookEventPost("Test", bind(&MacroPlugin::OnPreAsync, this, std::placeholders::_1), 1);
}

void MacroPlugin::onUnload()
{
}

template<typename T>
void write_pod(std::ofstream& out, T& t)
{
	out.write(reinterpret_cast<char*>(&t), sizeof(T));
}
template<typename T>
void read_pod(std::ifstream& in, T& t)
{
	in.read(reinterpret_cast<char*>(&t), sizeof(T));
}

void MacroPlugin::logPlaybackData(string filename)
{
	ofstream myfile;
	myfile.open(filename);
	for (unsigned int i = 2; i < playbackData.size(); i++)
	{
		myfile << playbackData.at(i) + "\n";
	}
	myfile.close();
	playbackData.clear();
}

void MacroPlugin::OnMacroCommand(std::vector<std::string> params)
{
	if (!gameWrapper->IsInTutorial())
	{
		cvarManager->log("You need to be in training to execute this");
		return;
	}
	std::string command = params.at(0);
	if (command.compare("plot_start") == 0)
	{
		if (currentStatus == PlotStatus_RECORDING)
		{
			currentStatus = PlotStatus_STOPPED;
			currentRecording.framecount = currentRecording.frames->size();
			logPlaybackData("./bakkesmod/recordings/playback_recording.data");
		}
		else if(currentStatus == PlotStatus_STOPPED)
		{
			currentStatus = PlotStatus_RECORDING;
			currentRecording.frames = std::make_shared<vector<std::shared_ptr<frame>>>(vector<std::shared_ptr<frame>>());
			ServerWrapper server = gameWrapper->GetGameEventAsServer();
			currentRecording.starttime = server.GetSecondsElapsed();
			OnRecordTick(); // Save first frame already
		}
	}
	else if (command.compare("plot_playback") == 0)
	{
		currentPlaybackIndex = 0;
		currentPlaybackFrame = currentRecording.frames->at(0);

		currentStatus = PlotStatus_PLAYING;
		ServerWrapper server = gameWrapper->GetGameEventAsServer();
		playbackStartTime = server.GetSecondsElapsed();
		auto players = gameWrapper->GetGameEventAsServer().GetCars();
		if (players.Count() > 0)
		{
			CarWrapper player0 = players.Get(0);
			player0.Stop();
			player0.SetLocation(currentPlaybackFrame->locationData);
			player0.SetRotation(currentPlaybackFrame->rotationData);
			player0.SetVelocity(currentPlaybackFrame->velocityData);
		}
	}
	else {
		if (params.size() < 2)
		{
			cvarManager->log("Usage: " + params.at(0) + " filename");
			return;
		}
		string filename = params.at(1);
		if (command.compare("plot_save") == 0)
		{
			//cvarManager->log("sizeof vector " + to_string(sizeof(Vector)));
			//cvarManager->log("sizeof rotator " + to_string(sizeof(Rotator)));
			//cvarManager->log("sizeof controllerinput " + to_string(sizeof(ControllerInput)));
			//cvarManager->log("sizeof header " + to_string(sizeof(float) + sizeof(float) + sizeof(float) + ));

			std::ofstream out(("./bakkesmod/recordings/" + filename).c_str(), ios::out | ios::trunc | ios::binary);
			write_pod(out, currentRecording.starttime);
			write_pod(out, currentRecording.endtime);
			int sizzze = currentRecording.frames->size();
			write_pod(out, sizzze);
			for (unsigned int i = 0; i < sizzze; i++) 
			{
				struct SaveFile
				{
					float Throttle = .0f;
					float Steer = .0f;
					float Pitch = .0f;
					float Yaw = .0f;
					float Roll = .0f;
					float DodgeForward = .0f;
					float DodgeStrafe = .0f;
					bool Jump = false;
					bool ActivateBoost = false;
					bool HoldingBoost = false;
					bool Handbrake = false;
					bool Jumped = false;
				};
				SaveFile sv;
				std::shared_ptr<frame> f = currentRecording.frames->at(i);
				write_pod(out, f->timestamp);
				//write_pod(out, f->input);
				sv.Throttle = f->input.Throttle;
				sv.Steer = f->input.Steer;
				sv.Pitch = f->input.Pitch;
				sv.Yaw = f->input.Yaw;
				sv.Roll = f->input.Roll;
				sv.DodgeForward = f->input.DodgeForward;
				sv.DodgeStrafe = f->input.DodgeStrafe;
				sv.Jump = f->input.Jump;
				sv.ActivateBoost = f->input.ActivateBoost;
				sv.HoldingBoost = f->input.HoldingBoost;
				sv.Handbrake = f->input.Handbrake;
				sv.Jumped = f->input.Jumped;
				/*write_pod(out, f->input.Throttle);
				write_pod(out, f->input.Steer);
				write_pod(out, f->input.Pitch);
				write_pod(out, f->input.Yaw);
				write_pod(out, f->input.Roll);
				write_pod(out, f->input.DodgeForward);
				write_pod(out, f->input.DodgeStrafe);
				bool jump = f->input.Jump;
				write_pod(out, jump);
				bool ActivateBoost = f->input.ActivateBoost;
				write_pod(out, ActivateBoost);
				bool HoldingBoost = f->input.HoldingBoost;
				write_pod(out, HoldingBoost);
				bool Handbrake = f->input.Handbrake;
				write_pod(out, Handbrake);
				bool Jumped = f->input.Jumped;
				write_pod(out, Jumped);*/
				write_pod(out, sv);
				write_pod(out, f->locationData);
				write_pod(out, f->rotationData);
				write_pod(out, f->velocityData);
			}
			out.flush();
			out.close();
		}
		else if (command.compare("plot_load") == 0) 
		{
			if (!file_exists("./bakkesmod/recordings/" + filename)) {
				cvarManager->log("./bakkesmod/recordings/" + filename + " does not exist");
				return;
			}
			std::ifstream in(("./bakkesmod/recordings/" + filename).c_str(), ios::binary);
			currentRecording.frames = std::make_shared<vector<std::shared_ptr<frame>>>(vector<std::shared_ptr<frame>>());
			read_pod(in, currentRecording.starttime);
			read_pod(in, currentRecording.endtime);
			read_pod(in, currentRecording.framecount);
			for (unsigned int i = 0; i < currentRecording.framecount; i++) {
				std::shared_ptr<frame> f(new frame());
				read_pod(in, f->timestamp);
				read_pod(in, f->input);
				read_pod(in, f->locationData);
				read_pod(in, f->rotationData);
				read_pod(in, f->velocityData);
				currentRecording.frames->push_back(f);
			}
			in.close();
		}
	}


}

void MacroPlugin::OnPreAsync(std::string funcName)
{
	//cvarManager->log("preasync");
	if (currentStatus == PlotStatus_RECORDING)
	{
		OnRecordTick();
	}
	else if (currentStatus == PlotStatus_PLAYING)
	{
		OnPlaybackTick();
	}
}

void MacroPlugin::OnPostAsync(std::string funcName)
{
	//cvarManager->log("postasync");
}

void MacroPlugin::OnRecordTick()
{
	ServerWrapper server = gameWrapper->GetGameEventAsServer();
	auto players = gameWrapper->GetGameEventAsServer().GetCars();
	if (players.Count() > 0)
	{
		CarWrapper player0 = players.Get(0);
		ControllerInput ci = player0.GetInput();
		std::shared_ptr<frame> fr(new frame());
		fr->timestamp = server.GetSecondsElapsed();
		fr->input = ci;
		fr->locationData = player0.GetLocation();
		fr->rotationData = player0.GetRotation();
		fr->velocityData = player0.GetVelocity();
		currentRecording.frames->push_back(fr);

		auto location = player0.GetLocation();
		string data_string = to_string(server.GetSecondsElapsed()) + "," + to_string(location.X) + "," + to_string(location.Y) + "," + to_string(location.Z);
		playbackData.push_back(data_string);
	}
}

void MacroPlugin::OnPlaybackTick()
{
	ServerWrapper server = gameWrapper->GetGameEventAsServer();
	auto players = gameWrapper->GetGameEventAsServer().GetCars();
	if (players.Count() > 0)
	{
		//while (server.GetSecondsElapsed() - playbackStartTime > currentPlaybackFrame->timestamp - currentRecording.starttime)
			
		if (currentPlaybackIndex > currentRecording.frames->size() - 1)
		{
			int nameIndex = 0;
			string filename = "./bakkesmod/recordings/playback_" + to_string(nameIndex) + ".data";
			while (file_exists(filename)) {
				nameIndex++;
				filename = "./bakkesmod/recordings/playback_" + to_string(nameIndex) + ".data";
			}
			logPlaybackData(filename);
			currentStatus = PlotStatus_STOPPED;
			return;
		}
		currentPlaybackFrame = currentRecording.frames->at(currentPlaybackIndex);
		//cvarManager->log("Frame & steer: " + to_string(currentPlaybackIndex) + " - " + to_string(currentPlaybackFrame->input.Steer));

		//cvarManager->log("boost & brake: " + to_string(currentPlaybackFrame->input.HoldingBoost) + " - " + to_string(currentPlaybackFrame->input.Handbrake));
		//std::shared_ptr<frame> nextFrame = currentRecording.frames->at(currentPlaybackIndex + 1);
		//frame interpedFrame = interp(currentPlaybackFrame, nextFrame, (currentPlaybackFrame->timestamp - currentRecording.starttime) - (server.GetSecondsElapsed() - playbackStartTime));
		auto player0 = players.Get(0);
		
		//player0.SetInput(currentPlaybackFrame->input);
		auto location = player0.GetLocation();
		string data_string = to_string(server.GetSecondsElapsed()) + "," + to_string(location.X) + "," + to_string(location.Y) + "," + to_string(location.Z);
		playbackData.push_back(data_string);
		static ControllerInput input;// = new ControllerInput();
		memcpy(&input, &currentPlaybackFrame->input, sizeof(ControllerInput));
		gameWrapper->OverrideParams(&input, sizeof(ControllerInput));
		currentPlaybackIndex++;
	}
}

frame MacroPlugin::interp(std::shared_ptr<frame> f1, std::shared_ptr<frame> f2, float elaps)
{
	float frameDiff = f2->timestamp - f1->timestamp;
	frame newFrame;

	memcpy(&newFrame, f1.get(), sizeof(frame));
	return newFrame;
	//No interpolatioooooooooon
	newFrame.input.ActivateBoost = f1->input.ActivateBoost;
	newFrame.input.Handbrake = f1->input.Handbrake;
	newFrame.input.HoldingBoost = f1->input.HoldingBoost;
	newFrame.input.Jump = f1->input.Jump;
	newFrame.input.Jumped = f1->input.Jumped;
	newFrame.input.DodgeStrafe = f1->input.DodgeStrafe;
	newFrame.input.DodgeForward = f1->input.DodgeForward;
	newFrame.input.Pitch = f1->input.Pitch + ((f2->input.Pitch - f1->input.Pitch) * elaps / frameDiff);
	newFrame.input.Yaw = f1->input.Yaw + ((f2->input.Yaw - f1->input.Yaw) * elaps / frameDiff);
	newFrame.input.Roll = f1->input.Roll + ((f2->input.Roll - f1->input.Roll) * elaps / frameDiff);
	newFrame.input.Throttle = f1->input.Throttle + ((f2->input.Throttle - f1->input.Throttle) * elaps / frameDiff);
	newFrame.input.Steer = f1->input.Steer + ((f2->input.Steer - f1->input.Steer) * elaps / frameDiff);
	return newFrame;
}
