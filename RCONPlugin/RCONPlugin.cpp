#include "RCONPlugin.h"
#include "bakkesmod/wrappers/PluginManagerWrapper.h"
#include "utils/parser.h"
#include <iostream>

BAKKESMOD_PLUGIN(RCONPlugin, "RCON plugin", "0.2", PLUGINTYPE_FREEPLAY)


bool RCONPlugin::is_authenticated(connection_ptr hdl) {
	if (auths.find(hdl) == auths.end())
	{
		connection_data conData;
		conData.authenticated = false;

		auths.insert(std::pair<connection_ptr, connection_data>(hdl, conData));
	}

	return auths[hdl].authenticated;
}

std::string stringify(std::vector<std::string> params)
{
	std::string str = "";
	for (size_t i = 1; i < params.size(); ++i)
	{
		str += params.at(i);
		if (i != params.size() - 1)
		{
			str += " ";
		}
	}
	str += "";

	return str;
}

void RCONPlugin::on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	connection_ptr con = s->get_con_from_hdl(hdl);
	try
	{
		const auto message = msg->get_payload();
		if (message.size() > 10000) return;
		auto input = parseConsoleInput(message);
		if (!is_authenticated(con))
		{
			if (input->size() > 0 && input->at(0).size() == 2 && input->at(0).at(0).compare("rcon_password") == 0)
			{
				std::string authKey = cvarManager->getCvar("rcon_password").getStringValue();
				std::string userAuthKey = input->at(0).at(1);
				if (!authKey.empty() && !userAuthKey.empty())
				{
					if (authKey.compare(userAuthKey) == 0)
					{
						auths[con].authenticated = true;
						s->send(hdl, "authyes", msg->get_opcode());
						delete input;
						return;
					}
				}
			}
			auths[con].tries += 1;
			s->send(hdl, "authno", msg->get_opcode());
			delete input;
			return;
		}
		//std::string payload = msg->get_payload();
		//cvarManager->log("Got from rcon: " + msg->get_payload());
		for (auto& inp : *input)
		{
			//cvarManager->log("Size: " + std::to_string(inp.size()));
			if (inp.size() == 0) continue;
			std::string command = inp.at(0);
			//cvarManager->log(command);
			if (is_allowed(command))
			{
				//is_allowed
				//Rebuild command
				std::string payload = msg->get_payload();
				//Doesn't seem neccessary? words in "" come thru in payload and sendback ok
				//replace(payload, "\"", "\\\"");
				gameWrapper->Execute([cmd = payload, &_cvarManager = cvarManager, log = *logRcon](GameWrapper* gw) {
					_cvarManager->executeCommand(cmd, log);
				});
			}
			else
			{
				std::stringstream ss;
				ss << "RCON tried to execute command that is not allowed (";
				ss << command;
				ss << ")!";
				cvarManager->log(ss.str());
				cvarManager->executeCommand("sendback \"ERR:illegal_command");
			}
		}
		delete input;
	}
	catch (const websocketpp::lib::error_code& e)
	{
		std::cout << "Echo failed because: " << e
			<< "(" << e.message() << ")" << std::endl;
		cvarManager->log("Websocket error " + (e.message()));
	}
}

bool RCONPlugin::is_allowed(std::string command)
{
	return std::any_of(allowed_commands.begin(), allowed_commands.end(), [command](const auto& re) { 
		std::smatch m;
		std::regex_match(command, m, re); 
		return m.size() > 0;
		});
}

void RCONPlugin::run_server()
{
	try
	{	
		if (ws_server.is_listening()) {
			cvarManager->log("Server already running");
			return;
		}
		cvarManager->log("Starting server");
		// Set logging settings
		
		//ws_server.init_asio();
		ws_server.set_access_channels(websocketpp::log::alevel::all);
		//ws_server.set_access_channels(websocketpp::log::alevel::access_core);
		//ws_server.set_access_channels(websocketpp::log::alevel::app);
		/*static std::ofstream wslog("ws.log.txt");
		static std::ofstream wserlog("ws.errlog.txt");
		ws_server.get_alog().set_ostream(&wslog);
		ws_server.get_elog().set_ostream(&wserlog);
		ws_server.get_alog().write(websocketpp::log::alevel::app, "ws_server constructor");
		ws_server.get_elog().write(websocketpp::log::alevel::app, "ws_server constructor elog");*/

		// Listen on port 9002
		int port = cvarManager->getCvar("rcon_port").getIntValue();
		if (port == NULL) port = 9002;

		ws_server.reset();
		ws_server.listen(port);

		cvarManager->log("RCON server started on port " + std::to_string(port));
		is_running = true;

		// Start the server accept loop
		ws_server.start_accept();

		// Start the ASIO io_service run loop
		ws_server.run();
	}
	catch (websocketpp::exception const & e)
	{
		cvarManager->log(e.code().message());
		cvarManager->log(std::to_string(e.code().value()));
		cvarManager->log(e.what());
	}
	catch (...)
	{
		cvarManager->log("Other exception");
	}
	is_running = false;
	cvarManager->log("rcon server shut down");
}

void RCONPlugin::onLoad()
{
	logRcon = std::make_shared<bool>(false);
	cvarManager->registerCvar("rcon_password", "password");


	// Initialize Asio
	ws_server.init_asio();

	// Register our message handler
	ws_server.set_message_handler(bind(&RCONPlugin::on_message, this, &ws_server, ::_1, ::_2));

	//server_thread = std::thread(std::bind(&RCONPlugin::run_server, this));
	//server_thread.detach();

	//cvarManager->registerCvar("rcon_port", "9002"); //Registered in the main dll now
	cvarManager->registerCvar("rcon_timeout", "5");
	cvarManager->registerCvar("rcon_log", "0", "Log all incoming rcon commands", true, true, 0, true, 1, true).bindTo(logRcon);
	cvarManager->registerCvar("rcon_enabled", "1", "Enable the RCON plugin", true, true, 0, true, 1, true).addOnValueChanged([this](std::string s, CVarWrapper newValue)
		{
			if (newValue.getBoolValue())
			{
				if (!ws_server.is_listening())
				{
					server_thread = std::thread(std::bind(&RCONPlugin::run_server, this));
					server_thread.detach();
				}
				else
				{
					cvarManager->log("RCON server is already running");
				}
			}
			else
			{
				shutdown_server();
			}
		});;



	cvarManager->registerNotifier("rcon_refresh_allowed", [this](std::vector<std::string> commands)
		{
			auto rcon_commands_path = gameWrapper->FixRelativePath("./bakkesmod/data/rcon_commands.cfg");
			allowed_commands.clear();
			if (std::filesystem::exists(rcon_commands_path))
			{
				std::ifstream rconFile(rcon_commands_path);
				for (std::string line; getline(rconFile, line);)
				{
					cvarManager->log("[RCON] Adding to allowlist: " + line);
					allowed_commands.push_back(std::regex(line));
				}
			}
			else
			{
				cvarManager->log("Could not load file: " + rcon_commands_path.string());
			}
		}, "Refreshes the list of allowed rcon commands", PERMISSION_ALL);

	cvarManager->registerNotifier("rcon_test_allowed", [this](std::vector<std::string> commands)
		{
			if (commands.size() < 2)
				return;
			cvarManager->log("Is allowed: " + std::string(is_allowed(commands.at(1)) ? "true" : "false"));
		}, "Tests if the given input is accepted by rcon", PERMISSION_ALL);

	cvarManager->registerNotifier("rcon_disconnect", [this](std::vector<std::string> commands) {
		for (auto& [connection, data] : auths)
		{
			if (connection->get_state() == websocketpp::session::state::open)
			{
				connection->close(1000, "User requested disconnect");
				cvarManager->log("Closing websocket connection");
			}
		}
	}, "Disconnects all rcon connections", PERMISSION_ALL);
	
	cvarManager->registerNotifier("rcon_start_server", [this](std::vector<std::string> commands) {
		cvarManager->executeCommand("rcon_enabled 1");
	}, "Start rcon server back up", PERMISSION_ALL);

	cvarManager->registerNotifier("rcon_kill_server", [this](std::vector<std::string> commands) {
		cvarManager->executeCommand("rcon_enabled 0");
	}, "Shutdown the rcon ws server", PERMISSION_ALL);

	cvarManager->registerNotifier("rcon_restart_server", [this](std::vector<std::string> commands) {
		cvarManager->executeCommand("rcon_enabled 0; rcon_enabled 1;");
	}, "Start rcon server back up after being shutdown", PERMISSION_ALL);

	cvarManager->registerNotifier("rcon_wsstatus", [this](std::vector<std::string> commands) {
		if (is_running)
		{
			cvarManager->log("Server is running");
		}
		
		if (ws_server.is_listening()) {
			cvarManager->log("Server is listening");
		}
		if (ws_server.is_server()) {
			cvarManager->log("ws is server");
		}
	}, "status check", PERMISSION_ALL);



	cvarManager->registerNotifier("sendback", [this](std::vector<std::string> commands) {
		if (commands.empty())
			return;

		for (auto& [connection, data] : auths)
		{
			if (is_authenticated(connection) && connection->get_state() == websocketpp::session::state::open)
			{
				connection->send(stringify(commands));
			}
		}
	}, "Sends text to all connected clients. Usage: sendback abc def ghi \"hij klm\"", PERMISSION_ALL); 

	cvarManager->registerNotifier("ws_inventory", [this](std::vector<std::string> commands) {
		if (commands.size() < 3)
			return;

		auto loadedPlugins = gameWrapper->GetPluginManager().GetLoadedPlugins();
		if (!std::any_of(loadedPlugins->begin(), loadedPlugins->end(), [](const auto& loadedPlugin) { return std::strcmp("betterinventoryexport", loadedPlugin->_details->fileName) == 0; }))
		{
			cvarManager->log("BetterInventoryExport is not loaded, installing!");
			cvarManager->executeCommand("bpm_install 155");
		}
		else
		{
			cvarManager->log("BetterInventoryExport is loaded. Using this for invent dump");
		}

		cvarManager->log("Sending inventory");
		for (auto& [connection, data] : auths)
		{
			if (is_authenticated(connection) && connection->get_state() == websocketpp::session::state::open)
			{
				std::string str = "invent_dump_better";
				for (int i = 1; i < 3; i++)
				{
					str += " \"" + commands.at(i) + "\"";
				}
				cvarManager->executeCommand(str);
				std::string format = commands.at(2).compare("csv") == 0 ? "csv" : "json"; //Dont want any path injection stuff
				std::ifstream ifs(gameWrapper->FixRelativePath("./bakkesmod/data/inventory." + format));
				std::string content((std::istreambuf_iterator<char>(ifs)),
					(std::istreambuf_iterator<char>()));

				std::string::size_type pos = 0; // Must initialize
				std::stringstream a;
				a << std::endl;
				std::string endl = a.str();
				while ((pos = content.find(endl, pos)) != std::string::npos)
				{
					content.erase(pos, endl.size());
				}
				auto ec = connection->send(content + "\n", websocketpp::frame::opcode::binary);

				cvarManager->log(ec.message());
			}
		}

	}, "Sends inventory dump to all connected clients. Usage: ws_inventory [all] (csv|json)", PERMISSION_ALL);

	cvarManager->executeCommand("rcon_refresh_allowed");

	/*
	This is a workaround since the rcon_enabled cvar is new, current clients will not have it in their config, so we notify
	in case the server isn't running. This way, users that have explicitly disabled RCON will not have a server running, but users
	who have it enabled will, and users who have the default value will have one running too.
	*/
	gameWrapper->SetTimeout([&](GameWrapper* gw)
		{
			if (!is_running)
			{
				cvarManager->getCvar("rcon_enabled").notify();
			}
		}, 5.f);
}

void RCONPlugin::shutdown_server() {
	if (ws_server.is_listening())
	{
		//server_running_mutex.unlock();
		ws_server.stop_listening();
		auth_iter iterator = auths.begin();
		while (iterator != auths.end())
		{
			if (iterator->first.get()->get_state() == websocketpp::session::state::open)
			{
				iterator->first->close(1000, "Shutting down BM");
				iterator->first->terminate(make_error_code(websocketpp::error::value::general));
			}
			iterator = auths.erase(iterator);
		}
		ws_server.stop();

		//if (!shut_down) {
		//	//this locks the looped thread
		//	server_running_mutex.lock();
		//}
		cvarManager->log("Server is shutting down");
	}
	if (server_thread.joinable())
	{
		server_thread.join();
	}
}

void RCONPlugin::onUnload()
{
	shut_down = true;
	shutdown_server();
	/*if (ws_server.is_listening())
	{
		ws_server.stop_listening();
	}
	ws_server.stop();*/
	//if (server_thread.joinable())
	//{
	//	server_thread.join();
	//}
	
}
