#pragma once
#include "game/classes.hpp"
#include <string>
#include <variant>

struct requestlist {
	rust::UnityWebRequest* request;
	float time;
};

namespace radar
{
	extern float last_update;
	//void player_data(bool do_ore, float update_rate, bool do_global);
	//void copy_link();
	//void new_link();
	//void default_var();
}