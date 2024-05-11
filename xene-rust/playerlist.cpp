#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <vector>
#include <utility>
#include <algorithm>
#include <limits>
#include "aimbot.h"
#include "game/classes.hpp"
#include "settings.h"
#include "global.h"

namespace playerlist {
	std::string ip = "unset";
	
	int port = 0;
	unsigned int mapsize = 0;
	unsigned int seed = 0;
	bool custom = false;
	bool is_connected = false;

	std::string get_grid(Vector3 pos, float world_size) {
		if (pos.x < -world_size / 2 || pos.x > world_size / 2 || pos.z < -world_size / 2 || pos.z > world_size / 2) {
			return "OOB";
		}

		char base_letter = 'A';
		std::string letter;
		int x = static_cast<int>(std::floor((pos.x + (world_size / 2)) / 146.3f));
		int z = static_cast<int>(std::floor(world_size / 146.3f) - std::floor((pos.z + (world_size / 2)) / 146.3f)) - 1;

		while (x >= 26) {
			letter.push_back(static_cast<char>(base_letter + (x / 26) - 1));
			x = x % 26;
		}
		letter.push_back(static_cast<char>(base_letter + x));

		if (z == -1) {
			return "OOB";
		}

		return letter + std::to_string(z);
	}

	void reset_server() {
		ip = "unset";
		port = 0;
		mapsize = 0;
		seed = 0;
		custom = false;
		is_connected = false;
	}

	void update_server(bool playerlist) {
		auto client = rust::get_client();
		if (client) {
			auto world = rust::get_world();
			if (world) {
				auto mip = client->get_ip();
				if (mip != "unset") {
					auto ms = world->get_mapsize();
					if (ms != 0) {
						if (variables::local_player) {
							if (!is_connected) {
								port = client->get_port();
								custom = world->is_custom();
								seed = world->get_seed();
								is_connected = true;
								mapsize = ms;
								ip = mip;
							}
						}
					}
				}
				else {
					reset_server();
				}
			}
			else {
				reset_server();
			}
		}
		else {
			reset_server();
		}
	}
}