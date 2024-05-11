#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include "hinclude.h"
#include "global.h"
#include <variant>
#include "playerlist.h"

bool areComponentsEqual(const Vector3& v1, const Vector3& v2) {
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}


namespace raids {
	std::vector<raidItem> activeRaids;
	Vector3 last_pos = { 0, 0, 0 };
	inline void get_raids()
	{
		auto effect_network = il2cpp::init_class(xorstr_("EffectNetwork"));
		if (!effect_network) {
			return;
		}

		auto effect = *reinterpret_cast<uintptr_t*>(effect_network + 0xB8);
		if (!effect) {
			return;
		}

		effect = *reinterpret_cast<uintptr_t*>(effect);
		if (!effect) {
			return;
		}

		Vector3 world_pos = *reinterpret_cast<Vector3*>(effect + 0x1C);

		//if (vis_empty(world_pos)) {
		//	return;
		//}

		auto PooledStringID = *reinterpret_cast<uint64_t*>(effect + 0x14);
		raidType explosion = { "deny", 0.0f };
		switch (PooledStringID) {
		case 1798302402:
			explosion = { "EXPLO", 1.0f };
			break;
		case 857997843:
			explosion = { "C4", 1.0f };
			break;
		case 3053816283:
			explosion = { "ROCKET", 1.0f };
			break;
		case 1289728008:
			explosion = { "SATCHEL", 1.0f };
			break;
		default:
			return;
		}
		if (explosion.type != "deny") {
			if (!areComponentsEqual(last_pos, world_pos)) {
				//std::cout << "id: " << PooledStringID << std::endl;


				last_pos = world_pos;
				bool exists = false;
				for (raidItem& item : activeRaids) {
					auto distancea = Math::Calc3D_Dist(world_pos, item.pos);
					if (120 > distancea) {
						item.time = global::getCurrentTimeMillis();
						exists = true;
						bool type_exists = false;

						for (raidType& explo : item.explosions) {
							if (explo.type == explosion.type) {
								explo.count = explo.count + 1;
								//std::cout << "Added count: " << explosion.type << std::endl;
								type_exists = true;
								break;
							}
						}

						if (!type_exists) {
							//std::cout << "Added raid type that didnt exist: " << explosion.type << std::endl;
							item.explosions.push_back(explosion);
						}
						break;
					}
				}
				if (!exists) {
					//std::cout << "x: " << world_pos.x << " y: " << world_pos.y << "z" << world_pos.z << std::endl;
					raidItem item_raid;
					item_raid.raid_id = global::GenerateRandomString(20);
					item_raid.pos = world_pos;
					item_raid.grid = playerlist::get_grid(world_pos, playerlist::mapsize);
					item_raid.explosions.push_back(explosion);
					item_raid.time = global::getCurrentTimeMillis();
					activeRaids.push_back(item_raid);
					//std::cout << "Added raid" << explosion.type << std::endl;
				}
			}
		}
	}
}