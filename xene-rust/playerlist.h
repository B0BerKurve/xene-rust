#pragma once
#include "game/classes.hpp"

namespace playerlist {
	extern int port;
	extern std::string ip;
	extern unsigned int mapsize;
	extern unsigned int seed;
	extern bool custom;
	extern bool is_connected;
	extern std::vector<playerlist_data> tcs;
	extern std::string filename;

	void file_to_array(std::string str);
	std::string get_grid(Vector3 pos, float world_size);
	void update_tcs();
	void reset_server();
	void update_server(bool playerlist);
}