#pragma once
#pragma comment( lib, "pluginsdk.lib" )
#define _WINSOCKAPI_
#define ASIO_STANDALONE
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"
#include <regex>
typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;
typedef server::connection_ptr connection_ptr;

struct connection_data
{
	bool authenticated = false;
	int tries = 0;
	std::vector<std::string> listeners;
};

typedef std::map<connection_ptr, connection_data >::iterator auth_iter;

class RCONPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	std::vector<std::regex> allowed_commands;

	std::shared_ptr<bool> logRcon;
	std::map<connection_ptr, connection_data> auths;
	std::mutex server_running_mutex;
	server ws_server;
	void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg);

	bool is_allowed(std::string command);

public:
	virtual void onLoad();
	virtual void onUnload();
	bool is_authenticated(connection_ptr hdl);
	void run_server();
	void stop_server();
};

