#pragma once

#include <sstream>
#include <map>
#include <vector>
#include "bakkesmod/wrappers/GameWrapper.h"
#include "bakkesmod/wrappers/cvarmanagerwrapper.h"
#include "json.hpp"
#include <stdlib.h>
#include <time.h>
#include "utils/parser.h"
#include <random>
#include <iterator>
using json = nlohmann::json;


struct VectorString {
	string x;
	string y;
	string z;

	VectorString(string xx, string yy, string zz) {
		x = xx;
		y = yy;
		z = zz;
	};
	VectorString() {}

	Vector getVector() {
		return Vector(get_safe_float(x), get_safe_float(y), get_safe_float(z));
	}
};
struct RotatorString {
	string pitch;
	string yaw;
	string roll;

	RotatorString(string p, string y, string r) {
		pitch = p;
		yaw = y;
		roll = r;
	};
	RotatorString() {}
	Rotator getRotator() {
		return Rotator(get_safe_float(pitch), get_safe_float(yaw), get_safe_float(roll));
	}
};

struct ball {
	VectorString location;
	VectorString velocity;
	VectorString angvel; //not used atm
	ball() {}
};

struct player {
	int player_idx = 0;
	VectorString location;
	RotatorString rotation;
	VectorString velocity;
	player() {}
};

struct start {
	vector<ball> balls;
	std::map<int, vector<player>> players;
	start() {}
};

struct dest {
	vector<VectorString> locations;
	dest() {}
};

struct options {
	bool shoot_on_ground = false;
	int shoot_on_goal = 0; // 0 = false, 1 = blue, 2 = orange
	bool disablemirror = false;
	bool use_velocity = false;
	string script = "";
	options() {}
};

struct speed {
	float min;
	float max;
	speed() {}
};

struct json_shot
{
	string name;
	start start;
	dest end;
	speed speed;
	options options;
	json_shot() {}
};


class JsonShot
{
private:
	string shotfile;
	json the_json;
	json_shot shot;

public:
	JsonShot();
	JsonShot(string shotfile);
	~JsonShot();
	void init(GameWrapper* gw, CVarManagerWrapper* cons);
	void setVelocity(GameWrapper * gw, CVarManagerWrapper * cons, Vector ballLoc);
	void set(GameWrapper* gw, CVarManagerWrapper* cons);
	json_shot getShot();
};

//template <typename I>
//I randomFromVector(Vector<I> vec)
//{
//	return randomFromVector(vec.begin(), vec.end());
//}
template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
	std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
	std::advance(start, dis(g));
	return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return select_randomly(start, end, gen);
}