#include "RCONPlugin.h"

#include "utils/parser.h"
#include <iostream>

BAKKESMOD_PLUGIN(RCONPlugin, "RCON plugin", "0.1", PLUGINTYPE_THREADED)

bool RCONPlugin::is_authenticated(connection_ptr hdl) {
	if (auths.find(hdl) == auths.end()) {
		connection_data conData;
		conData.authenticated = false;

		auths.insert(std::pair<connection_ptr, connection_data>(hdl, conData));
	}
	return auths[hdl].authenticated;
}

string stringify(std::vector<std::string> params)
{
	string str = "";
	for (size_t i = 1; i < params.size(); ++i)
	{
		str += params.at(i);
		if (i != params.size() - 1) {
			str += " ";
		}
	}
	str += "";
	return str;
}

void RCONPlugin::on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
	connection_ptr con = s->get_con_from_hdl(hdl);
	try {
		auto input = parseConsoleInput(msg->get_payload());
		if (!is_authenticated(con))
		{
			if (input->size() > 0 && input->at(0).size() == 2 && input->at(0).at(0).compare("rcon_password") == 0)
			{
				string authKey = cvarManager->getCvar("rcon_password").getStringValue();
				string userAuthKey = input->at(0).at(1);
				if (!authKey.empty() && !userAuthKey.empty())
				{
					if (authKey.compare(userAuthKey) == 0) {
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
		string payload = msg->get_payload();
		gameWrapper->Execute([payload, &_cvarManager = cvarManager](GameWrapper* gw) {
			_cvarManager->executeCommand(payload, false);
		});

		delete input;
	}
	catch (const websocketpp::lib::error_code& e) {
		std::cout << "Echo failed because: " << e
			<< "(" << e.message() << ")" << std::endl;
	}
}


void RCONPlugin::run_server() {
	try {
		// Set logging settings
		//ws_server.set_access_channels(websocketpp::log::alevel::all);
		//ws_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize Asio
		ws_server.init_asio();

		// Register our message handler
		ws_server.set_message_handler(bind(&RCONPlugin::on_message, this, &ws_server, ::_1, ::_2));

		// Listen on port 9002
		int port = cvarManager->getCvar("rcon_port").getIntValue();
		ws_server.listen(port);

		// Start the server accept loop
		ws_server.start_accept();

		// Start the ASIO io_service run loop
		ws_server.run();
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "other exception" << std::endl;
	}
}

void RCONPlugin::onLoad()
{
	cvarManager->registerCvar("rcon_password", "password");
	cvarManager->registerCvar("rcon_port", "9002"); //Registered in the main dll now
	cvarManager->registerCvar("rcon_timeout", "5");
	cvarManager->registerNotifier("sendback", [this](vector<string> commands) {
		if (commands.size() > 1)
		{
			for (auth_iter iterator = auths.begin(); iterator != auths.end(); iterator++)
			{
				if (is_authenticated(iterator->first) && iterator->first.get()->get_state() == websocketpp::session::state::open)
				{
					iterator->first->send(stringify(commands));
				}
			}
		}
	}); 


	//thread t(run_server);
	run_server();
	//ws_server.run();
}

void RCONPlugin::onUnload()
{
	ws_server.stop();
}
