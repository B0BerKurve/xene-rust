#pragma once
#include "hinclude.h"
#include "esp.h"
#include "misc.hpp"
#include "gui.h"
#include "config.h"
#include "aimbot.h"
#include "radar.h"
#include "raids.h"
#include "global.h"
#include "notifications.h"
#include "playerlist.h"
#include "./game/offsets.hpp"
#include "utils/memory.hpp"

Vector2 pos = Vector2(200, 200);
bool is_empty(const Vector3& vec) {
	return vec.x == 0 && vec.y == 0 && vec.z == 0;
}

std::string get_substring_after_slash(const std::string& input_str) {
	size_t posa = input_str.find_last_of('/');
	if (posa != std::string::npos) {
		return input_str.substr(posa + 1);
	}
	return input_str;
}

bool should_log = false;
std::string prefix = "assets/rust.ai/agents/"; //npc prefix
std::string prefix_bandit = "assets/prefabs/npc/"; //npc prefix 2

unsigned int hash_teamid(unsigned int x) {
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = (x >> 16) ^ x;
	return x;
}

Vector4 generate_rgba(unsigned int teamid) {
	unsigned int hashed = hash_teamid(teamid);
	float r = (hashed & 0xFF) / 255.0f;
	float g = ((hashed >> 8) & 0xFF) / 255.0f;
	float b = ((hashed >> 16) & 0xFF) / 255.0f;
	float a = 1.0f;  // You can also use hash to generate this if you want, but remember to divide by 255.0f
	return Vector4{ r, g, b, a };
}

void entity_loop()
{
	variables::client_entities = il2cpp::value(xorstr_("BaseNetworkable"), xorstr_("clientEntities"), false);
	//std::cout << variables::client_entities << std::endl;

	if (!variables::client_entities)
	{
		rust::LocalPlayer::ConsoleMessage(xorstr_("!client_entities"));
		return;
	}

	auto entity_realm = *reinterpret_cast<uintptr_t*>(variables::client_entities + 0x10);
	if (!entity_realm)
	{
		return;
	}

	auto buffer_list = *reinterpret_cast<uintptr_t*>(entity_realm + 0x28);
	if (!buffer_list)
	{
		return;
	}

	auto object_list_size = *reinterpret_cast<std::uint32_t*>(buffer_list + 0x10);

	auto object_list = *reinterpret_cast<uintptr_t*>(buffer_list + 0x18);
	if (!object_list)
	{
		return;
	}

	std::vector<rust::entity_base> tempEntity;

	std::map<std::string, std::tuple<std::string, Vector4, bool, float>> prefabToEntity = { //all (almost) world entitys
		{ "hemp-collectable.prefab", { "HEMP", std::get<Vector4>(ui::element["loot_hemp_color"].value), std::get<bool>(ui::element["loot_hemp"].value), std::get<float>(ui::element["loot_hemp_distance"].value) }},
		{ "stone-ore.prefab", {"STONE", std::get<Vector4>(ui::element["ore_stone_color"].value), std::get<bool>(ui::element["ore_stone"].value), std::get<float>(ui::element["ore_distance"].value)} },
		{ "metal-ore.prefab", {"METAL", std::get<Vector4>(ui::element["ore_metal_color"].value), std::get<bool>(ui::element["ore_metal"].value), std::get<float>(ui::element["ore_distance"].value)} },
		{ "sulfur-ore.prefab", {"SULFUR", std::get<Vector4>(ui::element["ore_sulfur_color"].value), std::get<bool>(ui::element["ore_sulfur"].value), std::get<float>(ui::element["ore_distance"].value)} },
		{ "wood-collectable.prefab", {"WOOD COLLECTIBLE", std::get<Vector4>(ui::element["wood_collectible_color"].value), std::get<bool>(ui::element["wood_collectible"].value), 500} },
		{ "stone-collectable.prefab", {"STONE COLLECTIBLE", std::get<Vector4>(ui::element["ore_stone_color"].value), (std::get<bool>(ui::element["ore_stone"].value) && std::get<bool>(ui::element["ore_collectible"].value)), std::get<float>(ui::element["ore_distance"].value)} },
		{ "metal-collectable.prefab", {"METAL COLLECTIBLE", std::get<Vector4>(ui::element["ore_metal_color"].value), (std::get<bool>(ui::element["ore_metal"].value) && std::get<bool>(ui::element["ore_collectible"].value)), std::get<float>(ui::element["ore_distance"].value)} },
		{ "sulfur-collectable.prefab", {"SULFUR COLLECTIBLE", std::get<Vector4>(ui::element["ore_sulfur_color"].value), (std::get<bool>(ui::element["ore_sulfur"].value) && std::get<bool>(ui::element["ore_collectible"].value)), std::get<float>(ui::element["ore_distance"].value)} },
		{ "tugboat.prefab", {"TUGBOAT", std::get<Vector4>(ui::element["vehicle_tugboat_color"].value), std::get<bool>(ui::element["vehicle_tugboat"].value), std::get<float>(ui::element["vehicle_tugboat_distance"].value)} },
		{ "attackhelicopter.entity.prefab", {"COMBAT HELI", std::get<Vector4>(ui::element["vehicle_attackmini_color"].value), std::get<bool>(ui::element["vehicle_attackmini"].value), std::get<float>(ui::element["vehicle_heli_distance"].value)} },
		{ "minicopter.entity.prefab", {"MINI", std::get<Vector4>(ui::element["vehicle_mini_color"].value), std::get<bool>(ui::element["vehicle_mini"].value), std::get<float>(ui::element["vehicle_heli_distance"].value)} },
		{ "scraptransporthelicopter.prefab", {"SCRAP HELI", std::get<Vector4>(ui::element["vehicle_scrap_color"].value), std::get<bool>(ui::element["vehicle_scrap"].value), std::get<float>(ui::element["vehicle_heli_distance"].value)} },
		{ "supply_drop.prefab", {"SUPPLY DROP", std::get<Vector4>(ui::element["loot_supply_color"].value), std::get<bool>(ui::element["loot_supply"].value), std::get<float>(ui::element["loot_supply_distance"].value)} },
		{ "rhib.prefab", {"RHIB", std::get<Vector4>(ui::element["vehicle_rhib_color"].value), std::get<bool>(ui::element["vehicle_rhib"].value), std::get<float>(ui::element["vehicle_boat_distance"].value)} },
		{ "rowboat.prefab", {"ROWBOAT", std::get<Vector4>(ui::element["vehicle_rowboat_color"].value), std::get<bool>(ui::element["vehicle_rowboat"].value), std::get<float>(ui::element["vehicle_boat_distance"].value)} },
		{ "crate_elite.prefab", {"ELITE CRATE", std::get<Vector4>(ui::element["loot_elitecrate_color"].value), std::get<bool>(ui::element["loot_elitecrate"].value), std::get<float>(ui::element["loot_crate_distance"].value)} },
		{ "crate_normal.prefab", {"MIL CRATE", std::get<Vector4>(ui::element["loot_milcrate_color"].value), std::get<bool>(ui::element["loot_milcrate"].value), std::get<float>(ui::element["loot_crate_distance"].value)} },
		{ "crate_normal_2.prefab", {"CRATE", std::get<Vector4>(ui::element["loot_normalcrate_color"].value), std::get<bool>(ui::element["loot_normalcrate"].value), std::get<float>(ui::element["loot_crate_distance"].value)} },
		{ "rifle.ak.diver (world)", {"AK47", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "rifle.ak.ice (world)", {"AK47", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "rifle.ak (world)", {"AK47", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "rifle.bolt (world)", {"BOLT", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "smg.2 (world)", {"CUSTOM SMG", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "shotgun.double (world)", {"DOUBLE BARREL", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "hmlmg (world)", {"HMLMG", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "rifle.lr300 (world)", {"LR-300", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "lmg.m249 (world)", {"M249", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "rifle.m39 (world)", {"M39", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "pistol.m92 (world)", {"M92", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "smg.mp5 (world)", {"MP5", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "multiplegrenadelauncher (world)", {"GRENADE LAUNCHER", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "smg.thompson (world)", {"THOMPSON", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "shotgun.spas12 (world)", {"SPAS-12", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "rifle.semiauto (world)", {"SEMI-RIFLE", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "pistol.semiauto (world)", {"SEMI-PISTOL", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "rocket.launcher (world)", {"LAUNCHER", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "pistol.revolver (world)", {"REVOLVER", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "pistol.python (world)", {"PYTHON", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "shotgun.pump (world)", {"PUMP SHOTGUN", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "pistol.prototype17 (world)", {"PROTOTYPE-17", std::get<Vector4>(ui::element["dropped_weapons_color"].value), std::get<bool>(ui::element["dropped_weapons"].value), std::get<float>(ui::element["dropped_weapons_distance"].value)} },
		{ "autoturret_deployed.prefab", {"TURRET", std::get<Vector4>(ui::element["traps_turret_color"].value), std::get<bool>(ui::element["traps_turret"].value), std::get<float>(ui::element["traps_turret_distance"].value)} },
		{ "sam_site_turret_deployed.prefab", {"SAMSITE", std::get<Vector4>(ui::element["traps_samsite_color"].value), std::get<bool>(ui::element["traps_samsite"].value), std::get<float>(ui::element["traps_samsite_distance"].value)} },
		{ "guntrap.deployed.prefab", {"SHOTGUN TRAP", std::get<Vector4>(ui::element["traps_shotgun_color"].value), std::get<bool>(ui::element["traps_shotgun"].value), std::get<float>(ui::element["traps_stationary_distance"].value)} },
		{ "flameturret.deployed.prefab", {"FLAME TURRET", std::get<Vector4>(ui::element["traps_flame_color"].value), std::get<bool>(ui::element["traps_flame"].value), std::get<float>(ui::element["traps_stationary_distance"].value)} },
		{ "testridablehorse.prefab", {"HORSE", std::get<Vector4>(ui::element["animal_color"].value), std::get<bool>(ui::element["animal_horse"].value), std::get<float>(ui::element["animal_horse_distance"].value)} },
		{ "bear.prefab", {"BEAR", std::get<Vector4>(ui::element["animal_color"].value), std::get<bool>(ui::element["animal_bear"].value), std::get<float>(ui::element["animal_distance"].value)} },
		{ "polarbear.prefab", {"BEAR", std::get<Vector4>(ui::element["animal_color"].value), std::get<bool>(ui::element["animal_bear"].value), std::get<float>(ui::element["animal_distance"].value)} },
		{ "stag.prefab", {"DEER", std::get<Vector4>(ui::element["animal_color"].value), std::get<bool>(ui::element["animal_deer"].value), std::get<float>(ui::element["animal_distance"].value)} },
		{ "wolf.prefab", {"WOLF", std::get<Vector4>(ui::element["animal_color"].value), std::get<bool>(ui::element["animal_wolf"].value), std::get<float>(ui::element["animal_distance"].value)} },
		{ "simpleshark.prefab", {"SHARK", std::get<Vector4>(ui::element["animal_color"].value), std::get<bool>(ui::element["animal_shark"].value), std::get<float>(ui::element["animal_distance"].value)} },
		{ "boar.prefab", {"BOAR", std::get<Vector4>(ui::element["animal_color"].value), std::get<bool>(ui::element["animal_boar"].value), std::get<float>(ui::element["animal_distance"].value)} },
		{ "workbench3.deployed.prefab", {"TIER 3", std::get<Vector4>(ui::element["loot_workbench_color"].value), std::get<bool>(ui::element["loot_workbench"].value), std::get<float>(ui::element["loot_workbench_distance"].value)} },
		{ "cupboard.tool.deployed.prefab", {"TC", std::get<Vector4>(ui::element["loot_tc_color"].value), std::get<bool>(ui::element["loot_tc"].value), std::get<float>(ui::element["loot_tc_distance"].value)} },
		{ "mushroom-cluster-6.prefab", {"MUSHROOM", std::get<Vector4>(ui::element["food_mushroom_color"].value), std::get<bool>(ui::element["food_mushroom"].value), std::get<float>(ui::element["food_distance"].value)} },
		{ "mushroom-cluster-5.prefab", {"MUSHROOM", std::get<Vector4>(ui::element["food_mushroom_color"].value), std::get<bool>(ui::element["food_mushroom"].value), std::get<float>(ui::element["food_distance"].value)} },
		{ "mushroom-cluster-4.prefab", {"MUSHROOM", std::get<Vector4>(ui::element["food_mushroom_color"].value), std::get<bool>(ui::element["food_mushroom"].value), std::get<float>(ui::element["food_distance"].value)} },
		{ "mushroom-cluster-3.prefab", {"MUSHROOM", std::get<Vector4>(ui::element["food_mushroom_color"].value), std::get<bool>(ui::element["food_mushroom"].value), std::get<float>(ui::element["food_distance"].value)} },
		{ "mushroom-cluster-2.prefab", {"MUSHROOM", std::get<Vector4>(ui::element["food_mushroom_color"].value), std::get<bool>(ui::element["food_mushroom"].value), std::get<float>(ui::element["food_distance"].value)} },
		{ "mushroom-cluster.prefab", {"MUSHROOM", std::get<Vector4>(ui::element["food_mushroom_color"].value), std::get<bool>(ui::element["food_mushroom"].value), std::get<float>(ui::element["food_distance"].value)} },
		{ "corn.entity.prefab", {"CORN", std::get<Vector4>(ui::element["food_corn_color"].value), std::get<bool>(ui::element["food_corn"].value), std::get<float>(ui::element["food_distance"].value)} },
		{ "potato-collectable.prefab", {"POTATO", std::get<Vector4>(ui::element["food_potato_color"].value), std::get<bool>(ui::element["food_potato"].value), std::get<float>(ui::element["food_distance"].value)} },
		{ "pumpkin.entity.prefab", {"PUMPKIN", std::get<Vector4>(ui::element["food_pumpkin_color"].value), std::get<bool>(ui::element["food_pumpkin"].value), std::get<float>(ui::element["food_distance"].value)} },
		{ "crate_normal_2_food.prefab", {"FOOD", std::get<Vector4>(ui::element["food_crate_color"].value), std::get<bool>(ui::element["food_crate"].value), std::get<float>(ui::element["food_distance"].value)} },
		{ "berry-black-collectable.prefab", {"BLACK BERRY", std::get<Vector4>(ui::element["food_berry_black_color"].value), std::get<bool>(ui::element["food_black_berry"].value), std::get<float>(ui::element["food_berry_distance"].value)} },
		{ "berry-blue-collectable.prefab", {"BLUE BERRY", std::get<Vector4>(ui::element["food_berry_blue_color"].value), std::get<bool>(ui::element["food_blue_berry"].value), std::get<float>(ui::element["food_berry_distance"].value)} },
		{ "berry-green-collectable.prefab", {"GREEN BERRY", std::get<Vector4>(ui::element["food_berry_green_color"].value), std::get<bool>(ui::element["food_green_berry"].value), std::get<float>(ui::element["food_berry_distance"].value)} },
		{ "berry-red-collectable.prefab", {"RED BERRY", std::get<Vector4>(ui::element["food_berry_red_color"].value), std::get<bool>(ui::element["food_red_berry"].value), std::get<float>(ui::element["food_berry_distance"].value)} },
		{ "berry-white-collectable.prefab", {"WHITE BERRY", std::get<Vector4>(ui::element["food_berry_white_color"].value), std::get<bool>(ui::element["food_white_berry"].value), std::get<float>(ui::element["food_berry_distance"].value)} },
		{ "berry-yellow-collectable.prefab", {"YELLOW BERRY", std::get<Vector4>(ui::element["food_berry_yellow_color"].value), std::get<bool>(ui::element["food_yellow_berry"].value), std::get<float>(ui::element["food_berry_distance"].value)} },
		{ "loot_barrel_1.prefab", {"BARREL", std::get<Vector4>(ui::element["loot_barrel_color"].value), std::get<bool>(ui::element["loot_barrel"].value), std::get<float>(ui::element["loot_barrel_distance"].value)} },
		{ "loot_barrel_2.prefab", {"BARREL", std::get<Vector4>(ui::element["loot_barrel_color"].value), std::get<bool>(ui::element["loot_barrel"].value), std::get<float>(ui::element["loot_barrel_distance"].value)} },
		{ "oil_barrel.prefab", {"OIL BARREL", std::get<Vector4>(ui::element["loot_oilbarrel_color"].value), std::get<bool>(ui::element["loot_oilbarrel"].value), std::get<float>(ui::element["loot_barrel_distance"].value)} },
		{ "diesel_collectable.prefab", {"DISEEL", std::get<Vector4>(ui::element["loot_diesel_color"].value), std::get<bool>(ui::element["loot_diesel"].value), 500} },
		{ "patrolhelicopter.prefab", {"ATTACK HELI", std::get<Vector4>(ui::element["attack_heli_color"].value), std::get<bool>(ui::element["attack_heli"].value), std::get<float>(ui::element["apc_distance"].value)} },
		{ "bradleyapc.prefab", {"BRADLEY", std::get<Vector4>(ui::element["bradley_color"].value), std::get<bool>(ui::element["attack_heli"].value), std::get<float>(ui::element["apc_distance"].value)} },
		{ "item_drop_backpack.prefab", {"BODY", std::get<Vector4>(ui::element["loot_bodybag_color"].value), std::get<bool>(ui::element["loot_bodybag"].value), std::get<float>(ui::element["loot_bodybag_distance"].value)} },
		{ "player_corpse.prefab", {"CORPSE", std::get<Vector4>(ui::element["loot_bodybag_color"].value), std::get<bool>(ui::element["loot_bodybag"].value), std::get<float>(ui::element["loot_bodybag_distance"].value)} },
		{ "bear-trap-deploy.prefab", {"BEAR TRAP", std::get<Vector4>(ui::element["traps_bear_color"].value), std::get<bool>(ui::element["traps_bear"].value), std::get<float>(ui::element["traps_stationary_distance"].value)} },
		{ "landmine.prefab", {"LANDMINE", std::get<Vector4>(ui::element["traps_landmine_color"].value), std::get<bool>(ui::element["traps_landmine"].value), std::get<float>(ui::element["traps_stationary_distance"].value)} },
		{ "recycler_static.prefab", {"RECYCLER", std::get<Vector4>(ui::element["recycler_color"].value), std::get<bool>(ui::element["recycler"].value), 400} },
		{ "codelockedhackablecrate.prefab", {"HACKABLE", std::get<Vector4>(ui::element["loot_hackable_color"].value), std::get<bool>(ui::element["loot_hackable"].value), std::get<float>(ui::element["loot_hackable_distance"].value)} },
		{ "codelockedhackablecrate_oilrig.prefab", {"HACKABLE", std::get<Vector4>(ui::element["loot_hackable_color"].value), std::get<bool>(ui::element["loot_hackable"].value), std::get<float>(ui::element["loot_hackable_distance"].value)} },
		{ "small_stash_deployed.prefab", {"STASH", std::get<Vector4>(ui::element["loot_stash_color"].value), std::get<bool>(ui::element["loot_stash"].value), std::get<float>(ui::element["loot_stash_distance"].value)}},
		{ "lock.code.prefab", {"CODELOCK", Vector4{0,0,0,0}, false, 4.5}},
	};

	rust::PlayerTeam* team_c = variables::local_player->get_team();
	ULONG local_team = variables::local_player->get_team_id();
	std::clock_t currentTime = global::getCurrentTimeMillis();
	std::vector<long> steamids;

	for (playerEnt& player : variables::players) {
		player.found = false;
	}

	for (int idx = 0; idx < object_list_size; idx++)
	{
		auto current_object = *reinterpret_cast<uintptr_t*>(object_list + (0x20 + (idx * sizeof(uint64_t))));
		if (!current_object)
		{
			continue;
		}

		auto object_unk = *reinterpret_cast<uintptr_t*>(current_object);
		if (!object_unk)
		{
			continue;
		}

		auto classname_ptr = *reinterpret_cast<uintptr_t*>(object_unk + 0x10);
		if (!classname_ptr)
		{
			continue;
		}

		auto base_object = *reinterpret_cast<uintptr_t*>(current_object + 0x10);
		if (!base_object)
		{
			continue;
		}

		auto object = *reinterpret_cast<uintptr_t*>(base_object + 0x30);
		if (!object)
		{
			continue;
		}

		if (idx == 0)
		{
			auto local_player_object_class = *reinterpret_cast<uintptr_t*>(object + 0x30);

			if (!local_player_object_class)
			{
				continue;
			}

			auto local_entity = *reinterpret_cast<uintptr_t*>(local_player_object_class + 0x18);

			if (!local_entity)
			{
				continue;
			}

			variables::local_player = *reinterpret_cast<rust::BasePlayer**>(local_entity + 0x28);
			if (variables::local_player->get_life_state() != 1) {
				if (variables::lcp.found) {
					variables::lcp.life_state = variables::local_player->get_life_state();
					if (currentTime > (variables::lcp.last_check + 100)) {
						variables::lcp.weapon = variables::local_player->get_held_item();
						variables::lcp.weapon_name = functions::ws2s(variables::lcp.weapon->get_name());
						variables::lcp.teamid = variables::local_player->get_team_id();
						variables::lcp.team = variables::local_player->get_team();
						variables::lcp.eyes = variables::local_player->get_eyes();
						variables::lcp.movement = variables::local_player->get_movement();
					}
				}
				else {
					player_local newLocal;
					newLocal.found = true;
					newLocal.life_state = variables::local_player->get_life_state();
					newLocal.player = (uintptr_t)variables::local_player;
					newLocal.name = functions::ws2s(variables::local_player->get_username());
					newLocal.weapon = variables::local_player->get_held_item();
					newLocal.weapon_name = functions::ws2s(newLocal.weapon->get_name());
					newLocal.team = variables::local_player->get_team();
					newLocal.teamid = variables::local_player->get_team_id();
					newLocal.userid = variables::local_player->get_uid();
					newLocal.last_check = global::getCurrentTimeMillis();
					newLocal.eyes = variables::local_player->get_eyes();
					newLocal.movement = variables::local_player->get_movement();

					variables::lcp = newLocal;
				}
			}
			else {
				variables::lcp.found = false;
			}

			if (variables::local_player->get_life_state())
			{
				continue;
			}

			Vector3 output;
			if (functions::getPositionFromView(output, variables::view_matrix)) {
				//std::cout << output.x << " | " << output.y << " | " << output.z << std::endl;
				variables::local_position = output;
			}
			else {
				//std::cout << "camera failure" << std::endl;
				variables::local_position = variables::local_player->get_visual_position(local_player_object_class);
			}

			//Vector3 local_position = variables::local_player->get_bone_position(48);
			//Vector3 CameraPos = Vector3(variables::view_matrix._41, variables::view_matrix._42, variables::view_matrix._43);
			
			continue;
		}
		variables::local_userid = variables::local_player->get_uid();
		functions::update_view_matrix();
		auto object_name_ptr = *reinterpret_cast<uintptr_t*>(object + 0x60);
		auto prefab_name = functions::read_ascii(object_name_ptr);
		const auto tag = *reinterpret_cast<std::uint16_t*>(object + 0x54);

		if (should_log) {
			std::cout << prefab_name << std::endl;
		}

		if (variables::local_player) {
			auto object_class = *reinterpret_cast<uintptr_t*>(object + 0x30);
			if (!object_class)
				return;
			if (tag == 6)
			{
				auto entity = *reinterpret_cast<uintptr_t*>(object_class + 0x18);
				if (!entity)
					return;

				auto player = *reinterpret_cast<rust::BasePlayer**>(entity + 0x28);
				if (!reinterpret_cast<uintptr_t>(player))
					return; //assets/prefabs/npc/

				if (player->get_life_state() != 1) {
					std::ptrdiff_t does_exist = functions::get_entity_index_in_list(variables::players, (uintptr_t)player);
					if (does_exist != -1) {
						playerEnt& player_ref = variables::players[does_exist];
						player_ref.found = true;
						if (player_ref.npc && std::get<bool>(ui::element["player_npc"].value) || !player_ref.npc) {
							player_ref.life_state = player->get_life_state();
							player_ref.health = player->get_health();
							player_ref.weapon = player->get_held_item();
							player_ref.position = player->get_model()->get_position();
							if (currentTime > (player_ref.last_check + 500)) {
								auto player_team = player->get_team();
								auto player_teamid = player->get_team_id();

								player_ref.knocked = player->is_knocked();
								player_ref.sleeping = player->is_sleeping();
								player_ref.teamid = player_teamid;
								player_ref.team = player_team;
								//player_ref.npc = (prefab_name.compare(0, prefix.length(), prefix) == 0) || (prefab_name.compare(0, prefix_bandit.length(), prefix_bandit) == 0);
								if ((prefab_name.compare(0, prefix.length(), prefix) == 0) || (prefab_name.compare(0, prefix_bandit.length(), prefix_bandit) == 0)) {
									player_ref.npc = true;
								}
								else {
									player_ref.npc = false;
								}
								if (player_teamid != 0 && player_teamid == local_team) {
									player_ref.teammate = true;
								}
								else {
									player_ref.teammate = false;
								}
								player_ref.is_connected = player->has_flag(256);
								player_ref.last_check = currentTime;
							}

							bool should_box = (player_ref.teammate == true) ? std::get<bool>(ui::element["team_box"].value) : std::get<bool>(ui::element["player_box"].value);
							bool should_name = (player_ref.teammate == true) ? std::get<bool>(ui::element["team_name"].value) : std::get<bool>(ui::element["player_name"].value);
							bool should_health = (player_ref.teammate == true) ? std::get<bool>(ui::element["team_health"].value) : std::get<bool>(ui::element["player_health"].value);
							bool should_weapon = (player_ref.teammate == true) ? std::get<bool>(ui::element["team_weapon"].value) : std::get<bool>(ui::element["player_weapon"].value);
							bool should_distance = (player_ref.teammate == true) ? std::get<bool>(ui::element["team_distance"].value) : std::get<bool>(ui::element["player_distance"].value);
							bool should_knocked = (player_ref.teammate == true) ? std::get<bool>(ui::element["team_knocked"].value) : std::get<bool>(ui::element["player_knocked"].value);
							bool should_sleeper = (player_ref.teammate == true) ? std::get<bool>(ui::element["team_sleep"].value) : std::get<bool>(ui::element["player_sleep"].value);
							bool should_skeleton = (player_ref.teammate == true) ? std::get<bool>(ui::element["team_skeleton"].value) : std::get<bool>(ui::element["player_skeleton"].value);
							float distance_s = (player_ref.teammate == true) ? std::get<float>(ui::element["team_distance_s"].value) : std::get<float>(ui::element["player_distance_s"].value);
							auto player_distance = Math::Calc3D_Dist(variables::local_position, player->get_model()->get_position());

							if (player_ref.npc == true) {
								distance_s = std::get<float>(ui::element["player_npc_distance_s"].value);
							}

							bool cap_distance = false;
							if (player_ref.teammate == true) {
								if (std::get<float>(ui::element["team_disable"].value) > player_distance) {
									cap_distance = true;
								}
							}

							if (distance_s > player_distance && !cap_distance) {
								steamids.push_back(player_ref.userid);
							}

							if (!should_knocked)
							{
								if (player_ref.knocked)
									continue;
							}
							if (!should_sleeper)
							{
								if (player_ref.sleeping) {
									if (player_ref.is_connected == false) {
										continue;
									}
								}
							}

							auto bounds = get_bounds(player, 2.f);
							Vector4 color = { std::get<Vector4>(ui::element["player_color"].value).x, std::get<Vector4>(ui::element["player_color"].value).y, std::get<Vector4>(ui::element["player_color"].value).z, std::get<Vector4>(ui::element["player_color"].value).w };

							if (player_ref.npc == true) {
								color.x = std::get<Vector4>(ui::element["player_npc_color"].value).x; color.y = std::get<Vector4>(ui::element["player_npc_color"].value).y, color.z = std::get<Vector4>(ui::element["player_npc_color"].value).z; color.w = std::get<Vector4>(ui::element["player_npc_color"].value).w;
							}
							else {
								if (player_ref.teammate == true && std::get<bool>(ui::element["team_check"].value)) {
									color.x = std::get<Vector4>(ui::element["team_check_color"].value).x; color.y = std::get<Vector4>(ui::element["team_check_color"].value).y, color.z = std::get<Vector4>(ui::element["team_check_color"].value).z; color.w = std::get<Vector4>(ui::element["team_check_color"].value).w;
								}
								else {
									if (player_ref.sleeping == true) {
										color.x = std::get<Vector4>(ui::element["player_sleep_color"].value).x;
										color.y = std::get<Vector4>(ui::element["player_sleep_color"].value).y;
										color.z = std::get<Vector4>(ui::element["player_sleep_color"].value).z;
										color.w = std::get<Vector4>(ui::element["player_sleep_color"].value).w;
									}
									else {
										if (player_ref.knocked == true) {
											color.x = std::get<Vector4>(ui::element["player_knocked_color"].value).x; color.y = std::get<Vector4>(ui::element["player_knocked_color"].value).y, color.z = std::get<Vector4>(ui::element["player_knocked_color"].value).z; color.w = std::get<Vector4>(ui::element["player_knocked_color"].value).w;
										}
										else {
											if (std::get<bool>(ui::element["player_vischeck"].value)) {
												Vector3 enemy_pos = player->get_bone_position(BoneList::head);
												if (variables::local_player->is_visible(enemy_pos)) {
													color.x = std::get<Vector4>(ui::element["player_viscolor"].value).x; color.y = std::get<Vector4>(ui::element["player_viscolor"].value).y, color.z = std::get<Vector4>(ui::element["player_viscolor"].value).z; color.w = std::get<Vector4>(ui::element["player_viscolor"].value).w;
												}
											}
										}
									}
								}
							}

							if (distance_s > player_distance && !cap_distance) {
								if (bounds.onscreen) {
									auto half = (bounds.right - bounds.left) / 2;
									float additional_distance = 0;
									//rust::Item* held_item = player->get_held_item();

									if (player_ref.npc == false && player_ref.sleeping == false && player_ref.knocked == false && player_ref.teammate == false) {
										if (std::get<bool>(ui::element["alert_players"].value)) {
											if (player_ref.render_check == false) {
												if (700 > player_ref.check_count) {
													std::string x_r = "West";
													std::string y_r = "South";
													if (player_ref.position.x > variables::local_position.x) {
														x_r = "East";
													}
													if (player_ref.position.z > variables::local_position.z) {
														y_r = "North";
													}

													if (player_ref.bot == false) {
														auto player_belt = player->get_hotbar_items();
														if (!player_belt.empty())
														{
															std::string response = "";
															for (int i = 0; i < player_belt.size(); i++)
															{
																auto item = player_belt.at(i);
																if (!item)
																	continue;

																if (item != NULL)
																{
																	std::string string_name = functions::ws2s(item->get_name());
																	if (string_name == "rocket.launcher") {
																		if (dropdowns::get_isSelected("Alert Options", "Launcher")) {
																			response = response + "Launcher, ";
																		}
																	}
																	if (string_name == "lmg.m249") {
																		if (dropdowns::get_isSelected("Alert Options", "M249")) {
																			response = response + "M249, ";
																		}
																	}
																	if (string_name == "explosive.timed") {
																		if (dropdowns::get_isSelected("Alert Options", "C4")) {
																			response = response + "C4, ";
																		}
																	}
																	player_ref.render_check = true;
																}
															}
															if (response != "") {
																if (response.rfind(", ", response.size() - 2) != std::string::npos) {
																	response = response.substr(0, response.size() - 2);
																}
																notifications::add_notification(player_ref.name + " Has a " + response + " towards " + y_r + " " + x_r, 5);
															}
														}
													}
													else {
														if (dropdowns::get_isSelected("Alert Options", "Bot")) {
															notifications::add_notification("A Bot named \"" + player_ref.name + "\" Has entered towards " + y_r + " " +	 x_r, 5);
															player_ref.render_check = true;
														}
													}
													player_ref.check_count = player_ref.check_count + 1;
												}
												else {
													player_ref.render_check = true;
												}
											}
										}
										if (std::get<bool>(ui::element["player_flags"].value)) {
											float y_offset = 2.0f;
											float box_size = 3;

											if (player_ref.bot == false) {
												auto player_belt = player->get_hotbar_items();
												auto player_wear = player->get_clothing_items();

												if (dropdowns::get_isSelected("Flags", "Team Flag") && std::get<bool>(ui::element["player_team"].value) && player_ref.teamid != 0) {
													color = generate_rgba(player_ref.teamid);
													functions::fill_box({ bounds.right + 3, (bounds.top) + y_offset, box_size, box_size }, color);
													y_offset = y_offset + box_size + 2;
												}
												if (dropdowns::get_isSelected("Flags", "Launcher Flag")) {
													if (!player_belt.empty())
													{
														for (int i = 0; i < player_belt.size(); i++)
														{
															auto item = player_belt.at(i);
															if (!item)
																continue;

															if (item != NULL)
															{
																std::string string_name = functions::ws2s(item->get_name());
																if (string_name == "rocket.launcher") {
																	functions::fill_box({ bounds.right + 3, (bounds.top) + y_offset, box_size, box_size }, { 1, 0.8470588235294118, 0.3333333333333333, 1 });
																	y_offset = y_offset + box_size + 2;
																	break;
																}
															}
														}
													}
												}
												if (dropdowns::get_isSelected("Flags", "NVG Flag")) {
													if (!player_wear.empty())
													{
														for (int i = 0; i < player_wear.size(); i++)
														{
															if (i > player_wear.size() - 1)
															{
																continue;
															}

															auto item = player_wear.at(i);
															if (!item)
																continue;

															if (item != NULL)
															{
																std::string string_name = functions::ws2s(item->get_name());
																if (string_name == "nightvisiongoggles") {
																	functions::fill_box({ bounds.right + 3, (bounds.top) + y_offset, box_size, box_size }, { 0.3333333333333333, 1, 0.4, 1 });
																	y_offset = y_offset + box_size + 2;
																	break;
																}
															}
														}
													}
												}
											}
											else {
												if (dropdowns::get_isSelected("Flags", "Bot Flag")) {
													functions::fill_box({ bounds.right + 3, (bounds.top) + y_offset, box_size, box_size }, { 1,0,0, 1 });
													y_offset = y_offset + box_size + 2;
												}
											}
										}
									}
									if (should_box)
									{
										esp::render_box(player, bounds, color);

									}
									if (should_skeleton && player_ref.npc == false)
									{
										if (130 > player_distance) {
											esp::do_skeleton(player, color);
										}
									}
									if (!player_ref.did_chams && !player_ref.teammate) {
										if (std::get<bool>(ui::element["player_chams"].value) && !player_ref.npc)
										{
											player_ref.did_chams = true;
											esp::do_chams(player, std::get<Vector4>(ui::element["player_chams_color"].value));
										}
									}
									if (should_weapon) {
										if (player_ref.weapon && (reinterpret_cast<uintptr_t>(player_ref.weapon)))
										{
											std::string item_name = functions::ws2s(player_ref.weapon->get_name());
											if (weapon_names.find(item_name) != weapon_names.end()) {
												item_name = weapon_names[item_name];
											}
											if (item_name.size() > 1) {
												additional_distance = 10;
												esp::render_text(Vector4{ bounds.left - 80.f, bounds.bottom + 2, 79.f + half * 2 + 80.f, 13 }, item_name, Vector4{ 1,1,1, 1 }, std::get<bool>(ui::element["outlines"].value), 10, true, 0, false);
											}
										}
									}
									if (should_distance)
									{
										std::string format_distance = "[" + std::to_string(int(player_distance)) + "m]";
										esp::render_text(Vector4{ bounds.left - 80.f, bounds.bottom + 2 + additional_distance, 79.f + half * 2 + 80.f, 13 }, format_distance, Vector4{ 1,1,1, 1 }, std::get<bool>(ui::element["outlines"].value), 10, true, 0, false);
									}

									if (should_health)
									{
										if (player_ref.teammate == true && std::get<bool>(ui::element["team_health_change"].value)) {
											esp::render_Health(player, bounds, true);
										}
										else {
											esp::render_Health(player, bounds, false);
										}
									}

									if (should_name) {
										esp::render_text(Vector4{ bounds.left - 80.f, bounds.top - 13, 79.f + half * 2 + 80.f, 13 }, player_ref.name, Vector4{ 1,1,1,1 }, std::get<bool>(ui::element["outlines"].value), 10, true, 0, false);
									}
								}
								else {
									if (std::get<bool>(ui::element["player_oof"].value)) {
										if (variables::local_player && !player_ref.npc && !player_ref.sleeping) {
											if (std::get<bool>(ui::element["player_oof_geared"].value)) {
												if (player_ref.bot == false && player_ref.npc == false) {
													auto player_belt = player->get_hotbar_items();
													if (!player_belt.empty())
													{
														for (int i = 0; i < player_belt.size(); i++)
														{
															auto item = player_belt.at(i);
															if (!item)
																continue;

															if (item != NULL)
															{
																if (item->is_gun()) {
																	esp::oof_arrows(player_ref.position, std::get<Vector4>(ui::element["player_oof_color"].value));
																	break;
																}
															}
														}
													}
												}
											}
											else {
												esp::oof_arrows(player_ref.position, std::get<Vector4>(ui::element["player_oof_color"].value));
											}
										}
									}
								}
							}
						}

						//if (variables::players[does_exist].did_chams) {

						//}

					}
					else {
						playerEnt player_entry;
						auto player_team = player->get_team();
						auto teamid = player->get_team_id();
						player_entry.entity = (uintptr_t)player;
						std::string string_name = functions::ws2s(player->get_username());
						if (string_name.length() > 15) {
							string_name = string_name.substr(0, 15);
						}
						player_entry.name = global::removeNonAlphabetical(string_name);
						player_entry.npc = false;
						player_entry.bot = false;
						if ((prefab_name.compare(0, prefix.length(), prefix) == 0) || (prefab_name.compare(0, prefix_bandit.length(), prefix_bandit) == 0)) {
							player_entry.npc = true;
							player_entry.teammate = false;
							player_entry.knocked = false;
							player_entry.sleeping = false;
						}
						else {
							player_entry.team = player_team;
							player_entry.knocked = player->is_knocked();
							player_entry.sleeping = player->is_sleeping();
							player_entry.teamid = teamid;
							player_entry.did_chams = false;
							if ((reinterpret_cast<uintptr_t>(team_c)) && teamid == local_team) {
								player_entry.teammate = true;
							}
							else {
								player_entry.teammate = false;
							}
							if (player->is_bot()) {
								player_entry.bot = true;
							}
						}

						player_entry.health = player->get_health();
						player_entry.weapon = player->get_held_item();
						player_entry.position = player->get_model()->get_position();
						player_entry.userid = player->get_uid();
						player_entry.found = true;
						player_entry.render_check = false;
						player_entry.is_connected = player->has_flag(256);
						player_entry.check_count = 0;
						player_entry.last_check = global::getCurrentTimeMillis();

						variables::players.push_back(player_entry);
					}
				}
				else {
					if (std::get<bool>(ui::element["player_dead"].value)) {
						Vector3 player_pos = player->get_visual_position(object_class);
						Vector2 draw_pos;
						if (functions::WorldToScreen(player_pos, draw_pos) == true) {
							std::string string_name = functions::ws2s(player->get_username());
							if (string_name.length() > 15) {
								string_name = string_name.substr(0, 15);
							}
							std::string distance_string = "";
							auto distancea = Math::Calc3D_Dist(variables::local_position, Vector3{ player_pos.x, player_pos.y, player_pos.z });
							if (std::get<bool>(ui::element["player_distance"].value)) {
								distance_string = " [" + global::floatToString(distancea) + "m]";
							}

							if (std::get<float>(ui::element["player_distance_s"].value) > distancea) {
								esp::render_text(Vector4{ draw_pos.x - 80.f, draw_pos.y, 160, 13 }, global::removeNonAlphabetical(string_name), Vector4{ 1,1,1,1 }, std::get<bool>(ui::element["outlines"].value), 10, true, 0, false);
								esp::render_text(Vector4{ draw_pos.x - 80.f, draw_pos.y + 10, 160, 13 }, "DEAD" + distance_string, Vector4{ 1,1,1,1 }, std::get<bool>(ui::element["outlines"].value), 10, true, 0, false);
							}
						}
					}
				}
			}
			else {
				if (std::get<bool>(ui::element["auto_upgrade"].value)) {
					std::string build_prefix = "assets/prefabs/building core/";
					if (prefab_name.compare(0, build_prefix.length(), build_prefix) == 0) {
						Vector3 world_pos = variables::local_player->get_visual_position(object_class);
						auto ent_distance = Math::Calc3D_Dist(variables::local_position, world_pos);
						if (5.f > ent_distance) {
							auto buildingblock = reinterpret_cast<rust::BuildingBlock*>(current_object);

							if (!std::get<bool>(ui::element["auto_upgrade_twig"].value) || std::get<bool>(ui::element["auto_upgrade_twig"].value) && buildingblock->get_grade() == 0) {
								int upgrade_grade = (std::get<int>(ui::element["upgrade_type"].value) + 1);
								if (buildingblock->can_upgrade(upgrade_grade, variables::local_player, 0)) {
									if (buildingblock->can_afford(upgrade_grade, variables::local_player, 0)) {
										if (currentTime > (global::get_float("bgrade") + 55)) {
											buildingblock->upgrade(upgrade_grade, variables::local_player, 0);
											global::float_map["bgrade"] = currentTime;
										}
									}
								}
							}
						}
					}
				}

				auto it = prefabToEntity.find(get_substring_after_slash(prefab_name));
				if (it != prefabToEntity.end()) {
					std::tuple<std::string, Vector4, bool, float>& value = it->second;
					std::string draw_name = std::get<0>(value);
					Vector4 color = std::get<1>(value);
					bool should_draw = std::get<2>(value);
					float draw_distance = std::get<3>(value);

					Vector3 world_pos = variables::local_player->get_visual_position(object_class);

					if (!is_empty(world_pos)) {
						auto ent = *reinterpret_cast<uintptr_t*>(base_object + 0x28);

						rust::entity_base currentent;
						currentent.position = world_pos;
						currentent.r = color.x;
						currentent.g = color.y;
						currentent.b = color.z;
						currentent.name = draw_name;
						currentent.entity = ent;

						tempEntity.push_back(currentent);

						auto ent_distance = Math::Calc3D_Dist(variables::local_position, world_pos);


						if (std::get<bool>(ui::element["auto_pickup"].value)) {
							if (variables::local_player && variables::lcp.life_state != 1) {
								if (currentTime > (global::get_float("pickup rpc") + 250)) {
									size_t pos = draw_name.find(" BERRY");
									std::string temp_name = (draw_name.rfind(" collectible") != std::string::npos) ? draw_name.substr(0, draw_name.rfind(" collectible")) : draw_name;
									if (dropdowns::get_isSelected("Pickup Options", draw_name) || dropdowns::get_isSelected("Pickup Options", temp_name) || pos != std::string::npos && dropdowns::get_isSelected("BERRY", draw_name)) {
										if (3 >= ent_distance) {
											variables::local_player->server_rpc(ent, ("Pickup"));
											global::float_map["pickup rpc"] = currentTime;
										}
									}
								}
							}
						}
						if (draw_name == "TC") {
							if (std::get<bool>(ui::element["loot_tc"].value) && std::get<bool>(ui::element["loot_tc_authorized"].value) && should_draw || std::get<bool>(ui::element["playerlist_enable"].value)) {
								auto tc = reinterpret_cast<rust::BuildingPrivelidge*>(ent);
								//if (std::get<bool>(ui::element["loot_tc_upkeep"].value)) {
								//	std::string upkeep = functions::format_time(tc->get_upkeep());
									//esp::render_text(Vector4{ draw_pos.x - 80.f, (draw_pos.y - 5) + y_offset, 160, 13 }, upkeep, color, std::get<bool>(ui::element["outlines"].value), 10, true);
									//y_offset = y_offset + 10;
								//}

								std::ptrdiff_t does_exist = functions::get_entity_index_in_list(variables::tc_list, ent);
								if (does_exist != -1) {
									std::clock_t elapsedMillis = currentTime - variables::tc_list[does_exist].last_check;
									if (elapsedMillis > 2500) {
										auto authList = tc->get_authorized();
										variables::tc_list[does_exist].player_list = authList;
										variables::tc_list[does_exist].entity = ent;
										variables::tc_list[does_exist].last_check = currentTime;
										variables::tc_list[does_exist].position = world_pos;
									}

								}
								else {
									auto authList = tc->get_authorized();
									vctr::toolCupboard new_tc;
									new_tc.player_list = authList;
									new_tc.entity = ent;
									new_tc.last_check = currentTime;
									new_tc.position = world_pos;
									variables::tc_list.push_back(new_tc);
								}
							}
						}

						if (should_draw) {
							Vector2 draw_pos;
							if (functions::WorldToScreen(world_pos, draw_pos) == true) {
								if (draw_distance > ent_distance) {
									std::string format_distance = "[" + std::to_string(int(ent_distance)) + "m]";
									bool continue_drawing = true;
									if (ent) {
										if (draw_name == "MINI" || draw_name == "SCRAP HELI" || draw_name == "COMBAT HELI") {
											std::ptrdiff_t does_exist = functions::get_entity_index_in_list(variables::helis, ent);
											if (does_exist == -1) {
												vctr::otherEntity new_entity;
												new_entity.entity = ent;
												new_entity.name = draw_name;
												new_entity.position = world_pos;
												new_entity.last_check = currentTime;
												variables::helis.push_back(new_entity);
											} else {
												variables::helis[does_exist].last_check = currentTime;
												variables::helis[does_exist].position = world_pos;
												variables::helis[does_exist].color = color;
											}
										}
										else if (draw_name == "TC") {
											std::ptrdiff_t does_exist = functions::get_entity_index_in_list(variables::tc_list, ent);
											if (does_exist != -1) {
												if (std::get<bool>(ui::element["loot_tc_authorized"].value)) {
													float y_offset = 10;
													for (int i = 0; i < variables::tc_list[does_exist].player_list.size(); i++) {
														auto player = variables::tc_list[does_exist].player_list[i];
														esp::render_text(Vector4{ draw_pos.x - 80.f, (draw_pos.y - 5) + y_offset, 160, 13 }, player.name, color, std::get<bool>(ui::element["outlines"].value), 10, true, ent_distance, std::get<bool>(ui::element["fade_out"].value));
														y_offset = y_offset + 10;
													}
												}
											}
										} else if (draw_name == "STASH") {
											auto stash = reinterpret_cast<rust::StashContainer*>(ent);
											if (stash->is_hidden()) {
												esp::render_text(Vector4{ draw_pos.x - 80.f, (draw_pos.y - 5) + 10, 160, 13 }, "HIDDEN", color, std::get<bool>(ui::element["outlines"].value), 10, true, ent_distance, std::get<bool>(ui::element["fade_out"].value));
											}
										} else if (draw_name == "STONE" || draw_name == "METAL" || draw_name == "SULFUR") {
											if (std::get<bool>(ui::element["ore_melee_only"].value) && variables::lcp.found) {
												if (variables::lcp.weapon_name == "rock" || variables::lcp.weapon_name == "jackhammer" || variables::lcp.weapon_name == "pickaxe" || variables::lcp.weapon_name == "stone.pickaxe" || variables::lcp.weapon_name == "icepick.salvaged") {}
												else {
													continue_drawing = false;
												}
											}
										} else if (draw_name == "HACKABLE") {
											if (std::get<bool>(ui::element["loot_hackable_time"].value)) {
												auto hackable_crate = reinterpret_cast<rust::HackableLockedCrate*>(ent);
												std::ptrdiff_t does_exist = functions::get_entity_index_in_list(variables::hackablecrate_list, ent);
												if (does_exist != -1) {
													std::clock_t elapsedMillis = currentTime - variables::hackablecrate_list[does_exist].last_check;
													if (elapsedMillis >= 1000) {
														float crate_time = hackable_crate->get_time();
														if (crate_time > 0) {
															//std::cout << "crate: " << crate_time << std::endl;
															variables::hackablecrate_list[does_exist].last_check = currentTime;
															variables::hackablecrate_list[does_exist].time = crate_time;
														}
													}
													if ((900 - variables::hackablecrate_list[does_exist].time) > 0) {
														esp::render_text(Vector4{ draw_pos.x - 80.f, (draw_pos.y - 5) + 10, 160, 13 }, functions::format_time(900 - variables::hackablecrate_list[does_exist].time), color, std::get<bool>(ui::element["outlines"].value), 10, true, ent_distance, std::get<bool>(ui::element["fade_out"].value));
													}
													else {
														esp::render_text(Vector4{ draw_pos.x - 80.f, (draw_pos.y - 5) + 10, 160, 13 }, "UNLOCKED", color, std::get<bool>(ui::element["outlines"].value), 10, true, ent_distance, std::get<bool>(ui::element["fade_out"].value));
													}
												}
												else {
													vctr::hackableCrate new_crate;
													new_crate.entity = ent;
													new_crate.last_check = currentTime;
													new_crate.position = world_pos;
													new_crate.time = hackable_crate->get_time();
													variables::hackablecrate_list.push_back(new_crate);
												}
											}
										}
										else if (draw_name == "TURRET") {
											if (dropdowns::get_isSelected("Turret Options", "Auth List") || dropdowns::get_isSelected("Turret Options", "Non Auth Only") || dropdowns::get_isSelected("Turret Options", "Powered Only")) {
												auto turret = reinterpret_cast<rust::AutoTurret*>(ent);

												/*if (2.5f > ent_distance) {
													for (int i = 0; i < variables::players.size(); i++) {
														auto player = variables::players[i];
														if (player.entity != (uintptr_t)variables::local_player) {
															if (player.npc == false && player.sleeping == true && player.bot == false) {
																if ((currentTime - global::get_float("authorize")) > 200) {
																	variables::local_player->server_rpc(turret, xorstr("AssignToFriend"), player.userid, false);
																	global::float_map["authorize"] = currentTime;

																}
																break;
															}
														}
													}
												}*/

												std::ptrdiff_t does_exist = functions::get_entity_index_in_list(variables::turret_list, ent);
												if (does_exist != -1) {
													std::clock_t elapsedMillis = currentTime - variables::turret_list[does_exist].last_check;
													if (elapsedMillis > 2500) {

														//std::cout << "online: " << turret->is_online() << std::endl;

														auto authList = turret->get_authorized();
														variables::turret_list[does_exist].player_list = authList;
														variables::turret_list[does_exist].entity = ent;
														variables::turret_list[does_exist].last_check = currentTime;
														variables::turret_list[does_exist].position = world_pos;
														variables::turret_list[does_exist].is_authed = turret->is_authorized(authList);
													}
													if (dropdowns::get_isSelected("Turret Options", "Non Auth Only")) {
														if (variables::turret_list[does_exist].is_authed) {
															continue_drawing = false;
														}
													}
													if (dropdowns::get_isSelected("Turret Options", "Powered Only") && continue_drawing) {
														if (!turret->is_online()) {
															continue_drawing = false;
														}
													}
													if (continue_drawing) {
														if (dropdowns::get_isSelected("Turret Options", "Auth List")) {
															float y_offset = 10;

															for (int i = 0; i < variables::turret_list[does_exist].player_list.size(); i++) {
																auto player = variables::turret_list[does_exist].player_list[i];
																esp::render_text(Vector4{ draw_pos.x - 80.f, (draw_pos.y - 5) + y_offset, 160, 13 }, player.name, color, std::get<bool>(ui::element["outlines"].value), 10, true, ent_distance, std::get<bool>(ui::element["fade_out"].value));
																y_offset = y_offset + 10;
															}
														}
													}
												}
												else {
													auto authList = turret->get_authorized();
													vctr::turrets new_turrets;
													new_turrets.player_list = authList;
													new_turrets.entity = ent;
													new_turrets.last_check = currentTime;
													new_turrets.position = world_pos;
													new_turrets.is_authed = turret->is_authorized(authList);
													variables::turret_list.push_back(new_turrets);
												}
											}
										}
										else if (draw_name == "ATTACK HELI" && std::get<bool>(ui::element["attack_heli_health"].value)) {
											auto entity = *reinterpret_cast<uintptr_t*>(object_class + 0x18);
											if (entity) {
												auto apc = *reinterpret_cast<rust::BasePlayer**>(entity + 0x28);
												if (apc) {
													esp::render_apc(apc, draw_pos);
												}
											}
										}
									}
									if (continue_drawing) {
										esp::render_text(Vector4{ draw_pos.x - 80.f, draw_pos.y - 5, 160, 13 }, draw_name + " " + format_distance, color, std::get<bool>(ui::element["outlines"].value), 10, true, ent_distance, std::get<bool>(ui::element["fade_out"].value));
									}
									//esp::render_text(Vector4{ draw_pos.x - 80.f, draw_pos.y + 5, 160, 13 }, format_distance, color, std::get<bool>(ui::element["outlines"].value), 10);
								}
							}
						}
					}
				}
			}
		}
	}

	functions::remove_old_entries(variables::tc_list, currentTime, 3000.f);
	functions::remove_old_entries(variables::turret_list, currentTime, 3000.f);
	functions::remove_old_entries(variables::hackablecrate_list, currentTime, 3000.f);
	functions::remove_old_entries(variables::helis, currentTime, 300000.f);
	//functions::remove_old_entries(variables::players, currentTime, 1200.f);

	variables::players.erase(
		std::remove_if(variables::players.begin(), variables::players.end(),
			[](const playerEnt& player) {
				return !player.found;
			}),
		variables::players.end()
	);

	should_log = false;
	variables::entity_draw = tempEntity;

	prefabToEntity.clear();


	return;
}

bool sent_request = false;

void do_log() {
	should_log = true;
}

void get_tc() {
	float last_distance = 1000000;
	rust::BuildingPrivelidge* tc = nullptr; //auto tc = reinterpret_cast<rust::BuildingPrivelidge*>(ent);
	for (int i = 0; i < variables::entity_draw.size(); i++) {
		rust::entity_base ent = variables::entity_draw[i];
		if (ent.name == "TC") {
			Vector2 head2D;
			if (functions::WorldToScreen(ent.position, head2D)) {
				double distanceToCenter = Math::Calc2D_Dist(Vector2(variables::screen_width / 2, variables::screen_height / 2), head2D);
				if (last_distance > distanceToCenter) {
					last_distance = distanceToCenter;
					tc = reinterpret_cast<rust::BuildingPrivelidge*>(ent.entity);
				}
			}
		}
	}
	if (tc != nullptr) {
		if (variables::screen_width > last_distance && variables::screen_height > last_distance) {
			auto authed_users = tc->get_authorized();
			std::ostringstream tcOutput;
			int count = 0;

			for (const authList& player : authed_users) {
				//std::cout << "Name: " << player.name << ", UserID: " << player.userid << std::endl;
				tcOutput << "https://steamcommunity.com/profiles/" << global::steam_id_64(player.userid) << " - " << player.name << "\n";
				count = count + 1;
			}
			std::string tcOutputStr = tcOutput.str();

			global::CopyTextToClipboard(tcOutputStr);
			notifications::add_notification(std::to_string(count) + " Players copied to clipboard", 5);
		}
		else {
			notifications::add_notification("No TC found on screen", 5);
		}
	}
	else {
		notifications::add_notification("No TC found", 5);
	}
}

void save_config() {
	config::config_save(ui::config_name);
}

void load_config() {
	config::config_load(ui::config_option);
}

void delete_config() {
	config::config_delete(ui::config_option);
}

namespace ongui
{
	bool left;
	bool right;

	std::vector<std::string> combo = { "one", "two" };
	int comboLeftSelected;
	int comboRightSelected;
	float sliderFloatLeft;
	float sliderFloatRight;
	int sliderIntLeft;
	int sliderIntRight;
	int tab = 0;
	int subtab = 0;
	bool run_loop = true;

	std::vector<vctr::otherEntity> cached_samsites;
	bool did_mounted = false;

	rust::UIBlackoutOverlay* overlay = rust::get_overlay(UIBlackoutOverlays::HELMETSLIT);
	bool set_alpha = false;

	bool holding_fullbright = false;
	bool fullbright_active = false;
	bool disabledRunFromServer;
	bool doing_nearest_player = false;
	std::string text_box = "";
	std::string last_config = "";
	bool found_hotkey = false;
	bool toggle_stash = false;
	bool last_playerlist = false;
	bool first_loop = false;
	bool set_flying = false;


	Vector4 old_chams = Vector4(0,0,0,0);
	float old_fov = 0;

	inline void ongui_hook(void* instance)
	{
		auto skin = functions::get_draw_skin();
		variables::draw_label = *reinterpret_cast<uintptr_t*>(skin + 0x38);
		std::clock_t currentTime = global::getCurrentTimeMillis();

		functions::set_draw_alignment(variables::draw_label, 0x0);
		functions::set_draw_font_size(variables::draw_label, 12);
		functions::set_draw_color(Vector4(1, 1, 1, 1));

		variables::screen_width = functions::get_screen_width();
		variables::screen_height = functions::get_screen_height();

		const auto current = functions::get_current();
		const auto event_type = functions::get_event_type(current);
		const auto key_code = functions::get_keyCode(current);

		if (event_type == EventType::Repaint) {
			entity_loop();
			if (run_loop) {
				//radar::default_var();
				config::populate_configs();

				pos.x = variables::screen_width / 2 - (675/2);
				pos.y = variables::screen_height / 2 - (430/2);
				std::string content;
				
				run_loop = false;

			}
		}

		if (currentTime > (global::get_float("update server") + 5000)) {

			playerlist::update_server(std::get<bool>(ui::element["playerlist_enable"].value));

			last_playerlist = std::get<bool>(ui::element["playerlist_enable"].value);
			global::float_map["update server"] = currentTime;
			first_loop = true;
		}

		if (variables::local_player) {

		}
		
		std::clock_t elapsedMillis = currentTime - global::get_float("aimbot key");
		if (std::get<bool>(ui::element["aimbot"].value)) {
			if (functions::get_key(std::get<KeyCode>(ui::element["aimbot_key"].value))) {

				if (std::get<bool>(ui::element["aimbot_silent"].value)) {
					aimbot::run(true);
				}
				else {
					if (elapsedMillis > 15) {
						global::float_map["aimbot key"] = currentTime;
						aimbot::run(false);
					}
				}
			}
		}

		if (currentTime > (global::get_float("config hotkey") + 100)) {
			bool temp_hotkey = false;
			for (configHotkeys& item : config::config_hotkeys) {
				if (item.hotkey != KeyCode::Unbound) {
					if (item.config_name != config::loaded_config) {
						if (functions::get_key(item.hotkey)) {
							if (found_hotkey == false) {
								config::config_load(item.config_name);
							}
							temp_hotkey = true;
							found_hotkey = true;
						}
					}
				}
			}
			if (temp_hotkey == false) {
				found_hotkey = false;
			}
			global::float_map["config hotkey"] = currentTime;
		}

		if (event_type == EventType::KeyUp && key_code == KeyCode::Insert) {
			show_menu = !show_menu;
		}

		notifications::active_notifications.erase(
			std::remove_if(notifications::active_notifications.begin(), notifications::active_notifications.end(),
				[currentTime](const notificationItem& item) {
					return currentTime > (item.time + (item.length * 1000));
				}),
			notifications::active_notifications.end()
		);

		GUI::window_width = functions::get_screen_width();
		GUI::window_height = functions::get_screen_height();

		current_ = current;
		event_ = event_type;

		GUI::Input::Handle();

		float notification_x = 0;
		float notification_y = 10;
		float yoffset = 0;

		for (notificationItem& item : notifications::active_notifications) {
			std::clock_t time_left = (item.time + (item.length * 1000)) - currentTime;

			float opacity = (time_left > 2000) ? 1.0f : static_cast<float>(time_left) / 2000.0f;

			float box_width = item.text.size() * (7.5f);

			esp::render_text(Vector4{ notification_x + 4, (notification_y + yoffset) + 3, 500, 30 }, item.text, Vector4{ 1,1,1,opacity }, true, 20, false, 0, false);
			yoffset = yoffset + 44;
		}

		if (GUI::Window("", &pos, Vector2(675, 465), show_menu)) {
			GUI::VertButtonBackground(Vector2(100, 30));
			bool aim_tab = GUI::VertButtonTab("Aim", Vector2(100, 30), tab == 0);
			GUI::SameCol();
			bool visuals_tab = GUI::VertButtonTab("Visuals", Vector2(100, 30), tab == 1);
			GUI::SameCol();
			bool misc_tab = GUI::VertButtonTab("Misc", Vector2(100, 30), tab == 2);
			GUI::SameCol();
			bool config_tab = GUI::VertButtonTab("Config", Vector2(100, 30), tab == 3);
			GUI::SameCol();
			bool other_tab = GUI::VertButtonTab("Other", Vector2(100, 30), tab == 4);

			if (tab == 0) {
				GUI::Divider("Other", true);
				GUI::Checkbox("Insta eoka", &std::get<bool>(ui::element["insta_eoka"].value), true, true);
				GUI::Divider("RCS Options", true);
				GUI::Checkbox("Recoil adjust", &std::get<bool>(ui::element["enable_recoil"].value), true, true);
				if (std::get<bool>(ui::element["enable_recoil"].value)) {
					GUI::ComboBox({ "AK47", "M249", "HMLMG", "LR300", "THOMPS", "MP5", "CUSTOM", "M39", "PYTHON", "SAR", "REVOLVER", "M92" }, std::get<int>(ui::element["gun_recoil"].value), true);

					if (std::get<int>(ui::element["gun_recoil"].value) == 0) {
						GUI::Slider(xorstr_("AK Recoil"), &std::get<float>(ui::element["ak_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 1) {
						GUI::Slider(xorstr_("M249 Recoil"), &std::get<float>(ui::element["m249_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 2) {
						GUI::Slider(xorstr_("HMLMG Recoil"), &std::get<float>(ui::element["hmlmg_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 3) {
						GUI::Slider(xorstr_("LR300 Recoil"), &std::get<float>(ui::element["lr300_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 4) {
						GUI::Slider(xorstr_("THOMPSON Recoil"), &std::get<float>(ui::element["thompson_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 5) {
						GUI::Slider(xorstr_("MP5 Recoil"), &std::get<float>(ui::element["mp5_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 6) {
						GUI::Slider(xorstr_("CUSTOM Recoil"), &std::get<float>(ui::element["custom_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 7) {
						GUI::Slider(xorstr_("M39 Recoil"), &std::get<float>(ui::element["m39_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 8) {
						GUI::Slider(xorstr_("PYTHON Recoil"), &std::get<float>(ui::element["python_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 9) {
						GUI::Slider(xorstr_("SAR Recoil"), &std::get<float>(ui::element["sar_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 10) {
						GUI::Slider(xorstr_("REVOLVER Recoil"), &std::get<float>(ui::element["revolver_recoil"].value), true, 0.0f, 100.0f, false);
					}
					else if (std::get<int>(ui::element["gun_recoil"].value) == 11) {
						GUI::Slider(xorstr_("M92 Recoil"), &std::get<float>(ui::element["m92_recoil"].value), true, 0.0f, 100.0f, false);

					}
				}

				GUI::Checkbox("Spread Adjust", &std::get<bool>(ui::element["spread_adjust"].value), true, true);
				if (std::get<bool>(ui::element["spread_adjust"].value)) {
					GUI::Slider(xorstr_("Spread Value"), &std::get<float>(ui::element["spread_value"].value), true, 0.0f, 100.0f, false);
				}

				GUI::Divider("Aim Options", false);
				GUI::Checkbox("Aimbot", &std::get<bool>(ui::element["aimbot"].value), false, true);
				if (std::get<bool>(ui::element["aimbot"].value)) {
					GUI::Checkbox("Silent aim", &std::get<bool>(ui::element["aimbot_silent"].value), false, true);
					GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["aimbot_key"].value)], "aimbot_key", false);
					GUI::ComboBox({ "Head", "Chest", "Pelvis" }, std::get<int>(ui::element["aimbot_hitbox"].value), false);
					GUI::Checkbox("Aimbot Prediction", &std::get<bool>(ui::element["aimbot_prediction"].value), false, true);
					GUI::Checkbox("Draw Aim Pos", &std::get<bool>(ui::element["aimbot_draw"].value), false, true);
					GUI::Checkbox("Team Check", &std::get<bool>(ui::element["aimbot_teamcheck"].value), false, true);
					GUI::Checkbox("Aimbot Vis Check", &std::get<bool>(ui::element["aimbot_vischeck"].value), false, true);
					if (!std::get<bool>(ui::element["aimbot_silent"].value)) {
						GUI::Checkbox("Aimbot Smoothing", &std::get<bool>(ui::element["aimbot_smoothing"].value), false, true);
						if (std::get<bool>(ui::element["aimbot_smoothing"].value)) {
							GUI::Slider("Aimbot Smoothing", &std::get<float>(ui::element["aimbot_smoothingCount"].value), false, 0.0f, 1000.0f, false);
						}
					}

					GUI::Divider("FOV ", true);
					GUI::Checkbox("Aimbot FOV", &std::get<bool>(ui::element["aimbot_fov"].value), true, true);
					if (std::get<bool>(ui::element["aimbot_fov"].value)) {
						GUI::Slider("Aimbot FOV", &std::get<float>(ui::element["aimbot_fovCount"].value), true, 0.0f, 500.0f, false);
						GUI::Checkbox("Draw FOV", &std::get<bool>(ui::element["aimbot_fovDraw"].value), true, !std::get<bool>(ui::element["aimbot_fovDraw"].value));
						if (std::get<bool>(ui::element["aimbot_fovDraw"].value)) {
							GUI::Colorpicker("Draw FOV", &std::get<Vector4>(ui::element["aimbot_fovColor"].value), true, true);
						}
						GUI::Checkbox("Dynamic FOV", &std::get<bool>(ui::element["aimbot_fovdynamic"].value), true, true);
						if (std::get<bool>(ui::element["aimbot_fovdynamic"].value)) {
							GUI::Slider("Meter range", &std::get<float>(ui::element["aimbot_fovdynamicrange"].value), true, 0.0f, 300.0f, true);
							GUI::Slider("Multiplier %", &std::get<float>(ui::element["aimbot_fovdynamicCount"].value), true, 0.0f, 250.0f, true);
						}
						else {
							aimbot::dynamic_fov = std::get<float>(ui::element["aimbot_fovCount"].value);
						}
					}
				}
			}
			else if (tab == 1) {
				int selectedTabIndex = GUI::subTab({ "Enemy", "Team", "Loot", "Vehicle", "Animal", "Plants", "Other", "Traps"}, subtab);

				if (selectedTabIndex != -1) {
					subtab = selectedTabIndex;
				}

				if (subtab == 0) { //enemy players
					GUI::yPaddingFix();
					GUI::Divider("Enemy ESP", true);
					GUI::Checkbox("Player Box", &std::get<bool>(ui::element["player_box"].value), true, true);
					GUI::Checkbox("Player Skeleton", &std::get<bool>(ui::element["player_skeleton"].value), true, true);
					GUI::Checkbox("Player Health", &std::get<bool>(ui::element["player_health"].value), true, true);
					GUI::Checkbox("Player Name", &std::get<bool>(ui::element["player_name"].value), true, true);
					if (std::get<bool>(ui::element["player_name"].value)) {
						GUI::Checkbox("Color Code Team", &std::get<bool>(ui::element["player_team"].value), true, true);
					}
					GUI::Checkbox("Player Distance", &std::get<bool>(ui::element["player_distance"].value), true, true);
					GUI::Checkbox("Player Weapon", &std::get<bool>(ui::element["player_weapon"].value), true, true);
					GUI::Checkbox("Player Hotbar", &std::get<bool>(ui::element["player_hotbar"].value), true, true);
					GUI::Checkbox("Player Chams", &std::get<bool>(ui::element["player_chams"].value), true, !std::get<bool>(ui::element["player_chams"].value));
					if (std::get<bool>(ui::element["player_chams"].value)) {
						if (old_chams.x != std::get<Vector4>(ui::element["player_chams_color"].value).x || old_chams.y != std::get<Vector4>(ui::element["player_chams_color"].value).y || old_chams.z != std::get<Vector4>(ui::element["player_chams_color"].value).z || old_chams.w != std::get<Vector4>(ui::element["player_chams_color"].value).w) {
							for (int i = 0; i < variables::players.size(); i++) {
								variables::players[i].did_chams = false;
							}
						}
						old_chams = std::get<Vector4>(ui::element["player_chams_color"].value);
						GUI::Colorpicker("Player Chams", &std::get<Vector4>(ui::element["player_chams_color"].value), true, true);
					}

					GUI::Checkbox("Player Knocked", &std::get<bool>(ui::element["player_knocked"].value), true, !std::get<bool>(ui::element["player_knocked"].value));
					if (std::get<bool>(ui::element["player_knocked"].value)) {
						GUI::Colorpicker("Player Knocked", &std::get<Vector4>(ui::element["player_knocked_color"].value), true, true);
					}
					GUI::Checkbox("Player Asleep", &std::get<bool>(ui::element["player_sleep"].value), true, !std::get<bool>(ui::element["player_sleep"].value));
					if (std::get<bool>(ui::element["player_sleep"].value)) {
						GUI::Colorpicker("Player Asleep", &std::get<Vector4>(ui::element["player_sleep_color"].value), true, true);
					}
					GUI::Checkbox("Player Vischeck", &std::get<bool>(ui::element["player_vischeck"].value), true, true);
					GUI::Checkbox("Player Dead", &std::get<bool>(ui::element["player_dead"].value), true, !std::get<bool>(ui::element["player_dead"].value));
					if (std::get<bool>(ui::element["player_dead"].value)) {
						GUI::Colorpicker("Player Dead", &std::get<Vector4>(ui::element["player_dead_color"].value), true, true);
					}
					GUI::Divider("Misc ESP", true);
					GUI::Checkbox("NPC", &std::get<bool>(ui::element["player_npc"].value), true, !std::get<bool>(ui::element["player_npc"].value));
					if (std::get<bool>(ui::element["player_npc"].value)) {
						GUI::Colorpicker("NPC", &std::get<Vector4>(ui::element["player_npc_color"].value), true, true);
					}
					GUI::Divider("Enemy Other", false);
					GUI::Checkbox("Player Flags", &std::get<bool>(ui::element["player_flags"].value), false, true);
					if (std::get<bool>(ui::element["player_flags"].value)) {
						std::vector<std::string> flag_options = { "Team Flag", "Launcher Flag", "NVG Flag", "Bot Flag" };
						std::unordered_map<std::string, bool>& flag_dropdownstate = dropdowns::dropdown_states["Flags"];
						GUI::DropDown("Flags", flag_options, flag_dropdownstate, false, false);
					}

					GUI::Checkbox("Player OOF", &std::get<bool>(ui::element["player_oof"].value), false, !std::get<bool>(ui::element["player_oof"].value));
					if (std::get<bool>(ui::element["player_oof"].value)) {
						GUI::Colorpicker("Player OOF", &std::get<Vector4>(ui::element["player_oof_color"].value), false, true);
						GUI::Checkbox("Geared OOF Only", &std::get<bool>(ui::element["player_oof_geared"].value), false, true);
					}

					GUI::Divider("ESP Options", false);
					GUI::Slider("Player Distance", &std::get<float>(ui::element["player_distance_s"].value), false, 10.0f, 500.0f, true);
					if (std::get<bool>(ui::element["player_npc"].value)) {
						GUI::Slider("NPC Distance", &std::get<float>(ui::element["player_npc_distance_s"].value), false, 10.0f, 500.0f, true);
					}
					if (std::get<bool>(ui::element["player_hotbar"].value)) {
						GUI::Slider("Hotbar FOV", &std::get<float>(ui::element["player_hotbar_fov"].value), false, 10.0f, 800.0f, true);
						GUI::Slider("Hotbar Offset", &std::get<float>(ui::element["player_hotbar_offset"].value), false, 10.0f, 3000.0f, true);
					}
					GUI::Colorpicker("Player Color", &std::get<Vector4>(ui::element["player_color"].value), false, false);
					if (std::get<bool>(ui::element["player_vischeck"].value)) {
						GUI::Colorpicker("Player Visible Color", &std::get<Vector4>(ui::element["player_viscolor"].value), false, false);
					}
				}
				else if (subtab == 1) { //teammates
					GUI::yPaddingFix();
					GUI::Divider("Team ESP", true);
					GUI::Checkbox("Teammate Check", &std::get<bool>(ui::element["team_check"].value), true, !std::get<bool>(ui::element["team_check"].value));
					if (std::get<bool>(ui::element["team_check"].value)) {
						GUI::Colorpicker("Teammate Check", &std::get<Vector4>(ui::element["team_check_color"].value), true, true);
					}
					GUI::Checkbox("Teammate Box", &std::get<bool>(ui::element["team_box"].value), true, true);
					GUI::Checkbox("Teammate Skeleton", &std::get<bool>(ui::element["team_skeleton"].value), true, true);
					GUI::Checkbox("Teammate Health", &std::get<bool>(ui::element["team_health"].value), true, true);
					if (std::get<bool>(ui::element["team_health"].value)) {
						GUI::Checkbox("Change Health Bar Color", &std::get<bool>(ui::element["team_health_change"].value), true, !std::get<bool>(ui::element["team_health_change"].value));
						if (std::get<bool>(ui::element["team_health_change"].value)) {
							GUI::Colorpicker("Change Health Bar Color", &std::get<Vector4>(ui::element["team_health_change_color"].value), true, true);
						}
					}
					GUI::Checkbox("Teammate Name", &std::get<bool>(ui::element["team_name"].value), true, true);
					GUI::Checkbox("Teammate Distance", &std::get<bool>(ui::element["team_distance"].value), true, true);
					GUI::Checkbox("Teammate Weapon", &std::get<bool>(ui::element["team_weapon"].value), true, true);
					GUI::Checkbox("Teammate Knocked", &std::get<bool>(ui::element["team_knocked"].value), true, true);
					GUI::Checkbox("Teammate Asleep", &std::get<bool>(ui::element["team_sleep"].value), true, true);
					GUI::Divider("Team ESP Options", false);
					GUI::Slider("Disable Distance", &std::get<float>(ui::element["team_disable"].value), false, 0.0f, 150.0f, true);
					GUI::Slider("Max View Distance", &std::get<float>(ui::element["team_distance_s"].value), false, 10.0f, 500.0f, true);
					GUI::Colorpicker("Teammate Color", &std::get<Vector4>(ui::element["team_color"].value), true, false);
				}
				else if (subtab == 2) { //loot
					GUI::yPaddingFix();
					GUI::Divider("Ore ", true);
					GUI::Checkbox("Stone Ore", &std::get<bool>(ui::element["ore_stone"].value), true, !std::get<bool>(ui::element["ore_stone"].value));
					if (std::get<bool>(ui::element["ore_stone"].value)) {
						GUI::Colorpicker("Stone Ore", &std::get<Vector4>(ui::element["ore_stone_color"].value), true, true);
					}
					GUI::Checkbox("Metal Ore", &std::get<bool>(ui::element["ore_metal"].value), true, !std::get<bool>(ui::element["ore_metal"].value));
					if (std::get<bool>(ui::element["ore_metal"].value)) {
						GUI::Colorpicker("Metal Ore", &std::get<Vector4>(ui::element["ore_metal_color"].value), true, true);
					}
					GUI::Checkbox("Sulfur ore", &std::get<bool>(ui::element["ore_sulfur"].value), true, !std::get<bool>(ui::element["ore_sulfur"].value));
					if (std::get<bool>(ui::element["ore_sulfur"].value)) {
						GUI::Colorpicker("Sulfur Ore", &std::get<Vector4>(ui::element["ore_sulfur_color"].value), true, true);
					}

					if (std::get<bool>(ui::element["ore_sulfur"].value) || std::get<bool>(ui::element["ore_metal"].value) || std::get<bool>(ui::element["ore_stone"].value)) {
						GUI::Checkbox("Melee Equipped Only", &std::get<bool>(ui::element["ore_melee_only"].value), true, true);
						GUI::Checkbox("Collectibles", &std::get<bool>(ui::element["ore_collectible"].value), true, true);
					}

					GUI::Divider("Crates", true);
					GUI::Checkbox("Elite Crate", &std::get<bool>(ui::element["loot_elitecrate"].value), true, !std::get<bool>(ui::element["loot_elitecrate"].value));
					if (std::get<bool>(ui::element["loot_elitecrate"].value)) {
						GUI::Colorpicker("Elite Crate", &std::get<Vector4>(ui::element["loot_elitecrate_color"].value), true, true);
					}
					GUI::Checkbox("Mil Crate", &std::get<bool>(ui::element["loot_milcrate"].value), true, !std::get<bool>(ui::element["loot_milcrate"].value));
					if (std::get<bool>(ui::element["loot_milcrate"].value)) {
						GUI::Colorpicker("Mil Crate", &std::get<Vector4>(ui::element["loot_milcrate_color"].value), true, true);
					}
					GUI::Checkbox("Normal Crate", &std::get<bool>(ui::element["loot_normalcrate"].value), true, !std::get<bool>(ui::element["loot_normalcrate"].value));
					if (std::get<bool>(ui::element["loot_normalcrate"].value)) {
						GUI::Colorpicker("Normal Crate", &std::get<Vector4>(ui::element["loot_normalcrate_color"].value), true, true);
					}
					GUI::Checkbox("Hackable Crate", &std::get<bool>(ui::element["loot_hackable"].value), true, !std::get<bool>(ui::element["loot_hackable"].value));
					if (std::get<bool>(ui::element["loot_hackable"].value)) {
						GUI::Colorpicker("Hackable Crate", &std::get<Vector4>(ui::element["loot_hackable_color"].value), true, true);
						GUI::Checkbox("Show Time", &std::get<bool>(ui::element["loot_hackable_time"].value), true, true);
					}
					GUI::Divider("Barrels", true);
					GUI::Checkbox("Barrels", &std::get<bool>(ui::element["loot_barrel"].value), true, !std::get<bool>(ui::element["loot_barrel"].value));
					if (std::get<bool>(ui::element["loot_barrel"].value)) {
						GUI::Colorpicker("Barrels", &std::get<Vector4>(ui::element["loot_barrel_color"].value), true, true);
					}
					GUI::Checkbox("Oil Barrels", &std::get<bool>(ui::element["loot_oilbarrel"].value), true, !std::get<bool>(ui::element["loot_oilbarrel"].value));
					if (std::get<bool>(ui::element["loot_oilbarrel"].value)) {
						GUI::Colorpicker("Oil Barrels", &std::get<Vector4>(ui::element["loot_oilbarrel_color"].value), true, true);
					}
					GUI::Checkbox("Diesel Fuel", &std::get<bool>(ui::element["loot_diesel"].value), true, !std::get<bool>(ui::element["loot_diesel"].value));
					if (std::get<bool>(ui::element["loot_diesel"].value)) {
						GUI::Colorpicker("Diesel Fuel", &std::get<Vector4>(ui::element["loot_diesel_color"].value), true, true);
					}

					GUI::Divider("Special", false);
					GUI::Checkbox("Supply Drops", &std::get<bool>(ui::element["loot_supply"].value), false, !std::get<bool>(ui::element["loot_supply"].value));
					if (std::get<bool>(ui::element["loot_supply"].value)) {
						GUI::Colorpicker("Supply Drops", &std::get<Vector4>(ui::element["loot_supply_color"].value), false, true);
					}
					GUI::Checkbox("Body Bags", &std::get<bool>(ui::element["loot_bodybag"].value), false, !std::get<bool>(ui::element["loot_bodybag"].value));
					if (std::get<bool>(ui::element["loot_bodybag"].value)) {
						GUI::Colorpicker("Body Bags", &std::get<Vector4>(ui::element["loot_bodybag_color"].value), false, true);
					}

					GUI::Checkbox("Recyclers", &std::get<bool>(ui::element["recycler"].value), false, !std::get<bool>(ui::element["recycler"].value));
					if (std::get<bool>(ui::element["recycler"].value)) {
						GUI::Colorpicker("Recyclers", &std::get<Vector4>(ui::element["recycler_color"].value), false, true);
					}

					GUI::Checkbox("Wood Collectible", &std::get<bool>(ui::element["wood_collectible"].value), false, !std::get<bool>(ui::element["wood_collectible"].value));
					if (std::get<bool>(ui::element["wood_collectible"].value)) {
						GUI::Colorpicker("Wood Collectible", &std::get<Vector4>(ui::element["wood_collectible_color"].value), false, true);
					}

					GUI::Divider("ESP Options", false);

					GUI::Slider("Crate Distance", &std::get<float>(ui::element["loot_crate_distance"].value), false, 10.0f, 500.0f, true);
					GUI::Slider("Ore Distance", &std::get<float>(ui::element["ore_distance"].value), false, 10.0f, 500.0f, true);
					GUI::Slider("Barrel Distance", &std::get<float>(ui::element["loot_barrel_distance"].value), false, 10.0f, 500.0f, true);

					if (std::get<bool>(ui::element["loot_bodybag"].value)) {
						GUI::Slider("Bodybag Distance", &std::get<float>(ui::element["loot_bodybag_distance"].value), false, 10.0f, 500.0f, true);
					}
					if (std::get<bool>(ui::element["loot_supply"].value)) {
						GUI::Slider("Supply Drop Distance", &std::get<float>(ui::element["loot_supply_distance"].value), false, 10.0f, 4000.0f, true);
					}
					if (std::get<bool>(ui::element["loot_hemp"].value)) {
						GUI::Slider("Hemp Distance", &std::get<float>(ui::element["loot_hemp_distance"].value), false, 10.0f, 500.0f, true);
					}
					if (std::get<bool>(ui::element["loot_hackable"].value)) {
						GUI::Slider("Hackable Crate Distance", &std::get<float>(ui::element["loot_hackable_distance"].value), false, 10.0f, 5500.0f, true);
					}
				}
				else if (subtab == 3) { //vehicles
					GUI::yPaddingFix();
					GUI::Divider("Air", true);
					GUI::Checkbox("Minicopters", &std::get<bool>(ui::element["vehicle_mini"].value), true, !std::get<bool>(ui::element["vehicle_mini"].value));
					if (std::get<bool>(ui::element["vehicle_mini"].value)) {
						GUI::Colorpicker("Minicopters", &std::get<Vector4>(ui::element["vehicle_mini_color"].value), true, true);
					}
					GUI::Checkbox("Combat Helicopter", &std::get<bool>(ui::element["vehicle_attackmini"].value), true, !std::get<bool>(ui::element["vehicle_attackmini"].value));
					if (std::get<bool>(ui::element["vehicle_attackmini"].value)) {
						GUI::Colorpicker("Combat Helicopter", &std::get<Vector4>(ui::element["vehicle_attackmini_color"].value), true, true);
					}
					GUI::Checkbox("Scrap Helicopter", &std::get<bool>(ui::element["vehicle_scrap"].value), true, !std::get<bool>(ui::element["vehicle_scrap"].value));
					if (std::get<bool>(ui::element["vehicle_scrap"].value)) {
						GUI::Colorpicker("Scrap Helicopter", &std::get<Vector4>(ui::element["vehicle_scrap_color"].value), true, true);
					}
					GUI::Slider("Heli Linger Seconds", &std::get<float>(ui::element["vehicle_heli_linger"].value), true, 0.0f, 300.0f, true);

					GUI::Divider("Water ", true);
					GUI::Checkbox("RHIB", &std::get<bool>(ui::element["vehicle_rhib"].value), true, !std::get<bool>(ui::element["vehicle_rhib"].value));
					if (std::get<bool>(ui::element["vehicle_rhib"].value)) {
						GUI::Colorpicker("RHIB", &std::get<Vector4>(ui::element["vehicle_rhib_color"].value), true, true);
					}
					GUI::Checkbox("Row Boat", &std::get<bool>(ui::element["vehicle_rowboat"].value), true, !std::get<bool>(ui::element["vehicle_rowboat"].value));
					if (std::get<bool>(ui::element["vehicle_rowboat"].value)) {
						GUI::Colorpicker("Row Boat", &std::get<Vector4>(ui::element["vehicle_rowboat_color"].value), true, true);
					}
					GUI::Checkbox("Tugboat", &std::get<bool>(ui::element["vehicle_tugboat"].value), true, !std::get<bool>(ui::element["vehicle_tugboat"].value));
					if (std::get<bool>(ui::element["vehicle_tugboat"].value)) {
						GUI::Colorpicker("Tugboat", &std::get<Vector4>(ui::element["vehicle_tugboat_color"].value), true, true);
					}
					GUI::Divider("Land ", true);
					GUI::Checkbox("Train", &std::get<bool>(ui::element["vehicle_train"].value), true, !std::get<bool>(ui::element["vehicle_train"].value));
					if (std::get<bool>(ui::element["vehicle_train"].value)) {
						GUI::Colorpicker("Train", &std::get<Vector4>(ui::element["vehicle_train_color"].value), true, true);
					}
					GUI::Divider("ESP Options", false);
					if (std::get<bool>(ui::element["vehicle_tugboat"].value)) {
						GUI::Slider("TugBoat Distance", &std::get<float>(ui::element["vehicle_tugboat_distance"].value), false, 10.0f, 4000.0f, true);
					}
					if (std::get<bool>(ui::element["vehicle_train"].value)) {
						GUI::Slider("Train Distance", &std::get<float>(ui::element["vehicle_train_distance"].value), false, 10.0f, 500.0f, true);
					}

					GUI::Slider("Heli Distance", &std::get<float>(ui::element["vehicle_heli_distance"].value), false, 10.0f, 4000.0f, true);
					GUI::Slider("Boat Distance", &std::get<float>(ui::element["vehicle_boat_distance"].value), false, 10.0f, 500.0f, true);
				}
				else if (subtab == 4) { //animals
					GUI::yPaddingFix();
					GUI::Divider("Animal ESP", true);
					GUI::Checkbox("Horses", &std::get<bool>(ui::element["animal_horse"].value), true, true);
					GUI::Checkbox("Bear", &std::get<bool>(ui::element["animal_bear"].value), true, true);
					GUI::Checkbox("Deer", &std::get<bool>(ui::element["animal_deer"].value), true, true);
					GUI::Checkbox("Shark", &std::get<bool>(ui::element["animal_shark"].value), true, true);
					GUI::Checkbox("Boar", &std::get<bool>(ui::element["animal_boar"].value), true, true);
					GUI::Checkbox("Wolf", &std::get<bool>(ui::element["animal_wolf"].value), true, true);
					GUI::Divider("ESP Options", false);
					GUI::Slider("Animal Distance", &std::get<float>(ui::element["animal_distance"].value), false, 10.0f, 500.0f, true);
					GUI::Colorpicker("Animal Color", &std::get<Vector4>(ui::element["animal_color"].value), true, false);
				}
				else if (subtab == 5) { //food
					GUI::yPaddingFix();
					GUI::Divider("Plants", true);
					GUI::Checkbox("Hemp", &std::get<bool>(ui::element["loot_hemp"].value), true, !std::get<bool>(ui::element["loot_hemp"].value));
					if (std::get<bool>(ui::element["loot_hemp"].value)) {
						GUI::Colorpicker("Hemp", &std::get<Vector4>(ui::element["loot_hemp_color"].value), true, true);
					}
					GUI::Checkbox("Mushroom", &std::get<bool>(ui::element["food_mushroom"].value), true, !std::get<bool>(ui::element["food_mushroom"].value));
					if (std::get<bool>(ui::element["food_mushroom"].value)) {
						GUI::Colorpicker("Mushroom", &std::get<Vector4>(ui::element["food_mushroom_color"].value), true, true);
					}
					GUI::Checkbox("Corn", &std::get<bool>(ui::element["food_corn"].value), true, !std::get<bool>(ui::element["food_corn"].value));
					if (std::get<bool>(ui::element["food_corn"].value)) {
						GUI::Colorpicker("Corn", &std::get<Vector4>(ui::element["food_corn_color"].value), true, true);
					}
					GUI::Checkbox("Potato", &std::get<bool>(ui::element["food_potato"].value), true, !std::get<bool>(ui::element["food_potato"].value));
					if (std::get<bool>(ui::element["food_potato"].value)) {
						GUI::Colorpicker("Potato", &std::get<Vector4>(ui::element["food_potato_color"].value), true, true);
					}
					GUI::Checkbox("Pumpkin", &std::get<bool>(ui::element["food_pumpkin"].value), true, !std::get<bool>(ui::element["food_pumpkin"].value));
					if (std::get<bool>(ui::element["food_pumpkin"].value)) {
						GUI::Colorpicker("Pumpkin", &std::get<Vector4>(ui::element["food_pumpkin_color"].value), true, true);
					}
					GUI::Divider("Berries", true);
					GUI::Checkbox("Black Berry", &std::get<bool>(ui::element["food_berry_black"].value), true, !std::get<bool>(ui::element["food_berry_black"].value));
					if (std::get<bool>(ui::element["food_berry_black"].value)) {
						GUI::Colorpicker("Black Berry", &std::get<Vector4>(ui::element["food_berry_black_color"].value), true, true);
					}
					GUI::Checkbox("Blue Berry", &std::get<bool>(ui::element["food_berry_blue"].value), true, !std::get<bool>(ui::element["food_berry_blue"].value));
					if (std::get<bool>(ui::element["food_berry_blue"].value)) {
						GUI::Colorpicker("Blue Berry", &std::get<Vector4>(ui::element["food_berry_blue_color"].value), true, true);
					}
					GUI::Checkbox("Green Berry", &std::get<bool>(ui::element["food_berry_green"].value), true, !std::get<bool>(ui::element["food_berry_green"].value));
					if (std::get<bool>(ui::element["food_berry_green"].value)) {
						GUI::Colorpicker("Green Berry", &std::get<Vector4>(ui::element["food_berry_green_color"].value), true, true);
					}
					GUI::Checkbox("Yellow Berry", &std::get<bool>(ui::element["food_berry_yellow"].value), true, !std::get<bool>(ui::element["food_berry_yellow"].value));
					if (std::get<bool>(ui::element["food_berry_yellow"].value)) {
						GUI::Colorpicker("Yellow Berry", &std::get<Vector4>(ui::element["food_berry_yellow_color"].value), true, true);
					}
					GUI::Checkbox("Red Berry", &std::get<bool>(ui::element["food_berry_red"].value), true, !std::get<bool>(ui::element["food_berry_red"].value));
					if (std::get<bool>(ui::element["food_berry_red"].value)) {
						GUI::Colorpicker("Red Berry", &std::get<Vector4>(ui::element["food_berry_red_color"].value), true, true);
					}
					GUI::Checkbox("White Berry", &std::get<bool>(ui::element["food_berry_white"].value), true, !std::get<bool>(ui::element["food_berry_white"].value));
					if (std::get<bool>(ui::element["food_berry_white"].value)) {
						GUI::Colorpicker("White Berry", &std::get<Vector4>(ui::element["food_berry_white_color"].value), true, true);
					}
					GUI::Divider("Special", true);
					GUI::Checkbox("Food Crate", &std::get<bool>(ui::element["food_crate"].value), true, !std::get<bool>(ui::element["food_crate"].value));
					if (std::get<bool>(ui::element["food_crate"].value)) {
						GUI::Colorpicker("Food Crate", &std::get<Vector4>(ui::element["food_crate_color"].value), true, true);
					}
					GUI::Divider("ESP Options", false);
					GUI::Slider("Plant Distance", &std::get<float>(ui::element["food_distance"].value), false, 10.0f, 500.0f, true);
					GUI::Slider("Berry Distance", &std::get<float>(ui::element["food_berry_distance"].value), false, 10.0f, 500.0f, true);
				}
				else if (subtab == 6) {
					GUI::yPaddingFix();
					GUI::Divider("Building", true);
					GUI::Checkbox("Workbench", &std::get<bool>(ui::element["loot_workbench"].value), true, !std::get<bool>(ui::element["loot_workbench"].value));
					if (std::get<bool>(ui::element["loot_workbench"].value)) {
						GUI::Colorpicker("Workbench", &std::get<Vector4>(ui::element["loot_workbench_color"].value), true, true);
					}

					GUI::Checkbox("Stash", &std::get<bool>(ui::element["loot_stash"].value), true, !std::get<bool>(ui::element["loot_stash"].value));
					if (std::get<bool>(ui::element["loot_stash"].value)) {
						GUI::Colorpicker("Stash", &std::get<Vector4>(ui::element["loot_stash_color"].value), true, true);
					}
					
					GUI::Checkbox("TC", &std::get<bool>(ui::element["loot_tc"].value), true, !std::get<bool>(ui::element["loot_tc"].value));
					if (std::get<bool>(ui::element["loot_tc"].value)) {
						GUI::Colorpicker("TC", &std::get<Vector4>(ui::element["loot_tc_color"].value), true, true);
						GUI::Checkbox("TC Auth List", &std::get<bool>(ui::element["loot_tc_authorized"].value), true, true);
						GUI::Button("Grab TC Players", &get_tc, true);
					}
					GUI::Divider("Other", true);
					GUI::Checkbox("Bradley", &std::get<bool>(ui::element["bradley"].value), true, !std::get<bool>(ui::element["bradley"].value));
					if (std::get<bool>(ui::element["bradley"].value)) {
						GUI::Colorpicker("Bradley", &std::get<Vector4>(ui::element["bradley_color"].value), true, true);
					}
					GUI::Checkbox("Attack Heli", &std::get<bool>(ui::element["attack_heli"].value), true, !std::get<bool>(ui::element["attack_heli"].value));
					if (std::get<bool>(ui::element["attack_heli"].value)) {
						GUI::Colorpicker("Attack Heli", &std::get<Vector4>(ui::element["attack_heli_color"].value), true, true);
						GUI::Checkbox("Attack Heli Health", &std::get<bool>(ui::element["attack_heli_health"].value), true, true);
					}
					GUI::Checkbox("Outlines", &std::get<bool>(ui::element["outlines"].value), true, true);
					GUI::Checkbox("Fade Out", &std::get<bool>(ui::element["fade_out"].value), true, true);
					GUI::Checkbox("Dropped Weapons", &std::get<bool>(ui::element["dropped_weapons"].value), true, !std::get<bool>(ui::element["dropped_weapons"].value));
					if (std::get<bool>(ui::element["dropped_weapons"].value)) {
						GUI::Colorpicker("Dropped Weapons", &std::get<Vector4>(ui::element["dropped_weapons_color"].value), true, true);
					}
					GUI::Divider("ESP Options", false);
					if (std::get<bool>(ui::element["loot_tc"].value)) {
						GUI::Slider("TC Distance", &std::get<float>(ui::element["loot_tc_distance"].value), false, 10.0f, 500.0f, true);
					}
					if (std::get<bool>(ui::element["loot_workbench"].value)) {
						GUI::Slider("Workbench Distance", &std::get<float>(ui::element["loot_workbench_distance"].value), false, 10.0f, 500.0f, true);
					}
					if (std::get<bool>(ui::element["dropped_weapons"].value)) {
						GUI::Slider("Dropped Weapon Distance", &std::get<float>(ui::element["dropped_weapons_distance"].value), false, 10.0f, 500.0f, true);
					}
					GUI::Slider("APC Distance", &std::get<float>(ui::element["apc_distance"].value), false, 10.0f, 6000.0f, true);
					if (std::get<bool>(ui::element["loot_stash"].value)) {
						GUI::Slider("Stash Distance", &std::get<float>(ui::element["loot_stash_distance"].value), false, 10.0f, 6000.0f, true);
					}
				}
				else if (subtab == 7) {
					GUI::yPaddingFix();
					GUI::Divider("Traps", true);
					GUI::Checkbox("Shotgun Trap", &std::get<bool>(ui::element["traps_shotgun"].value), true, !std::get<bool>(ui::element["traps_shotgun"].value));
					if (std::get<bool>(ui::element["traps_shotgun"].value)) {
						GUI::Colorpicker("Shotgun Trap", &std::get<Vector4>(ui::element["traps_shotgun_color"].value), true, true);
					}
					GUI::Checkbox("Flame Turret", &std::get<bool>(ui::element["traps_flame"].value), true, !std::get<bool>(ui::element["traps_flame"].value));
					if (std::get<bool>(ui::element["traps_flame"].value)) {
						GUI::Colorpicker("Shotgun Trap", &std::get<Vector4>(ui::element["traps_flame_color"].value), true, true);
					}
					GUI::Divider("Hardpoints", true);
					GUI::Checkbox("Turrets", &std::get<bool>(ui::element["traps_turret"].value), true, !std::get<bool>(ui::element["traps_turret"].value));
					if (std::get<bool>(ui::element["traps_turret"].value)) {
						GUI::Colorpicker("Turrets", &std::get<Vector4>(ui::element["traps_turret_color"].value), true, true);

						std::vector<std::string> turret_options = { "Auth List", "Non Auth Only", "Powered Only" };
						std::unordered_map<std::string, bool>& turret_dropdownstate = dropdowns::dropdown_states["Turret Options"];
						GUI::DropDown("Turret Options", turret_options, turret_dropdownstate, true, false);

					}
					GUI::Checkbox("Samsite", &std::get<bool>(ui::element["traps_samsite"].value), true, !std::get<bool>(ui::element["traps_samsite"].value));
					if (std::get<bool>(ui::element["traps_samsite"].value)) {
						GUI::Colorpicker("Samsite", &std::get<Vector4>(ui::element["traps_samsite_color"].value), true, true);
					}

					GUI::Divider("World", true);

					GUI::Checkbox("Bear Trap", &std::get<bool>(ui::element["traps_bear"].value), true, !std::get<bool>(ui::element["traps_bear"].value));
					if (std::get<bool>(ui::element["traps_bear"].value)) {
						GUI::Colorpicker("Bear Trap", &std::get<Vector4>(ui::element["traps_bear_color"].value), true, true);
					}

					GUI::Checkbox("Landmine", &std::get<bool>(ui::element["traps_landmine"].value), true, !std::get<bool>(ui::element["traps_landmine"].value));
					if (std::get<bool>(ui::element["traps_landmine"].value)) {
						GUI::Colorpicker("Landmine", &std::get<Vector4>(ui::element["traps_landmine_color"].value), true, true);
					}


					if (std::get<bool>(ui::element["traps_turret"].value)) {
						GUI::Slider("Turret Distance", &std::get<float>(ui::element["traps_turret_distance"].value), false, 10.0f, 500.0f, true);
					}
					if (std::get<bool>(ui::element["traps_samsite"].value)) {
						GUI::Slider("Samsite Distance", &std::get<float>(ui::element["traps_samsite_distance"].value), false, 10.0f, 500.0f, true);
					}
					GUI::Slider("Trap Distance", &std::get<float>(ui::element["traps_stationary_distance"].value), false, 10.0f, 500.0f, true);
				}

				//GUI::Button("Log prefabs", &do_log, false);
			} else if (tab == 2) {
				GUI::Divider("Misc Options", false);
				GUI::Checkbox("Raid ESP", &std::get<bool>(ui::element["raid_esp"].value), false, !std::get<bool>(ui::element["raid_esp"].value));
				if (std::get<bool>(ui::element["raid_esp"].value)) {
					GUI::Colorpicker("Raid ESP", &std::get<Vector4>(ui::element["raid_esp_color"].value), true, true);
					GUI::Slider("Raid ESP Linger Seconds", &std::get<float>(ui::element["raid_esp_seconds"].value), false, 10.0f, 650.0f, true);
				}
				GUI::Checkbox("Admin flags", &std::get<bool>(ui::element["admin_enable"].value), false, true);
				if (std::get<bool>(ui::element["admin_enable"].value)) {
					GUI::Checkbox("On key", &std::get<bool>(ui::element["admin_key_enable"].value), false, true);
					if (std::get<bool>(ui::element["admin_key_enable"].value)) {
						GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["admin_key"].value)], "admin_key", false);
					}
				}
				GUI::Checkbox("Block Server Commands", &std::get<bool>(ui::element["admin_block_commands"].value), false, true);
				if (std::get<bool>(ui::element["admin_block_commands"].value)) {
					GUI::Checkbox("Only Block While Admin", &std::get<bool>(ui::element["admin_block_commands_adminonly"].value), false, true);
				}
				//GUI::Checkbox("Interactive Debug", &std::get<bool>(ui::element["interactive_enable"].value), false, true);
				//if (std::get<bool>(ui::element["interactive_enable"].value)) {
				//	GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["interactive_key"].value)], "interactive_key", false);
				//}

				GUI::Checkbox("Fullbright", &std::get<bool>(ui::element["fullbright_enable"].value), false, true);
				if (std::get<bool>(ui::element["fullbright_enable"].value)) {
					GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["fullbright_key"].value)], "fullbright_key", false);
				}

				GUI::Checkbox("Crosshair", &std::get<bool>(ui::element["crosshair_enable"].value), false, !std::get<bool>(ui::element["crosshair_enable"].value));
				if (std::get<bool>(ui::element["crosshair_enable"].value)) {
					GUI::Colorpicker("Crosshair", &std::get<Vector4>(ui::element["crosshair_color"].value), false, true);
				}
				GUI::Checkbox("Rocket Trajectory", &std::get<bool>(ui::element["rocket_trajectory"].value), false, true);

				GUI::Divider("Radar", true);
				GUI::Checkbox("Enable", &std::get<bool>(ui::element["radar_enable"].value), true, true);
				if (std::get<bool>(ui::element["radar_enable"].value)) {
					std::vector<std::string> radar_options = { "Ore", "Shared Radar" };

					std::unordered_map<std::string, bool>& radar_dropdownstate = dropdowns::dropdown_states["Radar Options"];
					GUI::DropDown("Radar Options", radar_options, radar_dropdownstate, true, false);

					/*if (!dropdowns::get_isSelected("Radar Options", "Shared Radar")) {
						GUI::Button("Change Link", &radar::new_link, true);
					}
					GUI::Button("Copy Link", &radar::copy_link, true);
					GUI::Slider("Radar Update Rate", &std::get<float>(ui::element["radar_updaterate"].value), true, 40.0f, 1500.0f, true);*/
				}
				GUI::Checkbox("In-Game Radar", &std::get<bool>(ui::element["radar"].value), true, true);
				if (std::get<bool>(ui::element["radar"].value)) {
					GUI::Checkbox("Radar Teammates", &std::get<bool>(ui::element["radar_teammates"].value), true, true);
				}

				GUI::Divider("Other", true);

				GUI::Checkbox("Get Nearest Player", &std::get<bool>(ui::element["nearest_player_clipboard"].value), true, true);
				if (std::get<bool>(ui::element["nearest_player_clipboard"].value)) {
					GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["nearest_player_clipboard_key"].value)], "nearest_player_clipboard_key", true);
				}

				GUI::Checkbox("Alerts", &std::get<bool>(ui::element["alert_players"].value), true, true);
				if (std::get<bool>(ui::element["alert_players"].value)) {
					std::vector<std::string> alert_options = { "Launcher", "C4", "M249", "Bot", "Online Samsites", "Offline Samsites" };
					std::unordered_map<std::string, bool>& alert_dropdownstate = dropdowns::dropdown_states["Alert Options"];
					GUI::DropDown("Alert Options", alert_options, alert_dropdownstate, true, false);
				}
			} else if (tab == 3) {
				/*GUI::Divider("Config", false);
				GUI::Button("Save config", &config::config_save, false);
				GUI::Button("Load config", &config::config_load, false);
				GUI::Button("Export config", &config::config_export, false);
				GUI::Button("Import config", &config::config_import, false);*/
				std::vector<std::string> my_string_array = { "Option 1", "Option 2", "Option 3" };
				GUI::BigRectangleWithStrings("Configs", config::config_options, false, ui::config_option);
				GUI::TextBox("Config Name", ui::config_name, false);

				GUI::Divider("Config", true);
				GUI::Button("Save config", &save_config, true);
				GUI::Button("Load config", &load_config, true);
				GUI::Button("Export config", &config::config_export, true);
				GUI::Button("Import config", &config::config_import, true);
				GUI::Button("Delete config", &delete_config, true);
				GUI::Checkbox("Config Hotkey", &std::get<bool>(ui::element["config_hotkey_enable"].value), true, true);
				if (std::get<bool>(ui::element["config_hotkey_enable"].value)) {
					GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["config_hotkey"].value)], "config_hotkey", true);
				}

				if (last_config != ui::config_option) {
					ui::config_name = ui::config_option;
					last_config = ui::config_option;
				}
			} else if (tab == 4) {
				GUI::Divider("Misc", false);
				//GUI::Checkbox("Font Change", &std::get<bool>(ui::element["font"].value), false, true);
				GUI::Checkbox("Auto Upgrade", &std::get<bool>(ui::element["auto_upgrade"].value), false, true);
				if (std::get<bool>(ui::element["auto_upgrade"].value)) {
					GUI::Checkbox("Twig Only", &std::get<bool>(ui::element["auto_upgrade_twig"].value), false, true);
					GUI::ComboBox({ "Wood", "Stone", "Metal", "HQM" }, std::get<int>(ui::element["upgrade_type"].value), false);
				}
				GUI::Checkbox("Remove Heavy Plate", &std::get<bool>(ui::element["remove_heavyplate"].value), false, true);
				GUI::Checkbox("Silent Stash", &std::get<bool>(ui::element["silent_stash"].value), false, true);
				if (std::get<bool>(ui::element["silent_stash"].value)) {
					GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["silent_stash_key"].value)], "silent_stash_key", false);
				}
				GUI::Checkbox("Auto Pickup", &std::get<bool>(ui::element["auto_pickup"].value), false, true);
				if (std::get<bool>(ui::element["auto_pickup"].value)) {
					std::vector<std::string> pickup_options = { "BERRY", "HEMP", "WOOD", "STONE", "METAL", "SULFUR", "CORN", "PUMPKIN", "MUSHROOM", "POTATO" };
					std::unordered_map<std::string, bool>& pickup_dropdownstate = dropdowns::dropdown_states["Pickup Options"];
					GUI::DropDown("Pickup Options", pickup_options, pickup_dropdownstate, false, false);
				}
				GUI::DangerCheckbox("Instant Revive Team", &std::get<bool>(ui::element["instant_revive"].value), false, true);
				if (std::get<bool>(ui::element["instant_revive"].value)) {
					GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["instant_revive_key"].value)], "instant_revive_key", false);
				}
				GUI::DangerCheckbox("Fake Shoot", &std::get<bool>(ui::element["fake_shoot"].value), false, true);
				if (std::get<bool>(ui::element["fake_shoot"].value)) {
					GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["fake_shoot_key"].value)], "fake_shoot_key", false);
				}
				GUI::Divider("Movement", true);
				GUI::DangerCheckbox("Flyhack", &std::get<bool>(ui::element["flyhack"].value), true, true);
				if (std::get<bool>(ui::element["flyhack"].value)) {
					GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["flyhack_key"].value)], "flyhack_key", true);
				}
				GUI::Checkbox("Infinite Jump", &std::get<bool>(ui::element["infinite_jump"].value), true, true);
				GUI::Checkbox("No Collision", &std::get<bool>(ui::element["no_collision"].value), true, true);
				GUI::Checkbox("FOV Changer", &std::get<bool>(ui::element["fov_change"].value), true, true);
				if (std::get<bool>(ui::element["fov_change"].value)) {
					GUI::Slider("FOV", &std::get<float>(ui::element["fov_change_value"].value), true, 40.0f, 140.0f, true);
				}
				GUI::DangerCheckbox("No Fall Damage", &std::get<bool>(ui::element["nofall"].value), true, true);
				GUI::DangerCheckbox("Suicide", &std::get<bool>(ui::element["suicide"].value), true, true);
				if (std::get<bool>(ui::element["suicide"].value)) {
					GUI::Hotkey(keyCodeToString[std::get<KeyCode>(ui::element["suicide_key"].value)], "suicide_key", true);
				}
			}
			//else if (tab == 5) {
			//	GUI::Checkbox("Playerlist", &std::get<bool>(ui::element["playerlist_enable"].value), false, true);
			//}

			GUI::ExecutePostOperations();

			if (aim_tab) {
				tab = 0;
			}
			else if (visuals_tab) {
				tab = 1;
			}
			else if (misc_tab) {
				tab = 2;
			}
			else if (config_tab) {
				tab = 3;
			}
			else if (other_tab) {
				tab = 4;
			}
			//else if (playerlist_tab) {
			//	tab = 5;
			//}

			last_config = ui::config_option;

			Vector2 cursorPos = GUI::CursorPos();
		}

		if (std::get<bool>(ui::element["alert_players"].value)) {
			if (dropdowns::get_isSelected("Alert Options", "Online Samsites") || dropdowns::get_isSelected("Alert Options", "Offline Samsites")) {
				if (currentTime > (global::get_float("samsite") + 2000)) {
					bool log_mounted = false;
					bool is_mounted = variables::local_player->is_mounted();
					if (is_mounted) {
						if (!did_mounted) {
							for (const auto& entity : variables::entity_draw) {
								auto distancea = Math::Calc3D_Dist(variables::local_position, entity.position);
								if (entity.name == "MINI" && 2 > distancea || entity.name == "SCRAP HELI" && distancea > 5 || entity.name == "COMBAT HELI" && distancea > 3) {
									log_mounted = true;
								}
							}
							did_mounted = true;
						}
						std::vector<vctr::otherEntity> temp_samsites;

						for (const auto& entity : variables::entity_draw) {
							if (entity.name == "SAMSITE") {
								std::string x_r = "West";
								std::string y_r = "South";
								if (entity.position.x > variables::local_position.x) {
									x_r = "East";
								}
								if (entity.position.z > variables::local_position.z) {
									y_r = "North";
								}

								bool is_online = reinterpret_cast<rust::SamSite*>(entity.entity)->is_online();
								std::string start_string = (is_online ? "[Online]" : "[Offline]");

								if (is_online && dropdowns::get_isSelected("Alert Options", "Online Samsites") || !is_online && dropdowns::get_isSelected("Alert Options", "Offline Samsites")) {
									if (log_mounted) {
										notifications::add_notification(start_string + " Samsite towards " + y_r + " " + x_r, 5);
									}
									else {
										std::ptrdiff_t does_exist = functions::get_entity_index_in_list(cached_samsites, entity.entity);
										if (does_exist == -1) {
											notifications::add_notification(start_string + " Samsite towards " + y_r + " " + x_r, 5);
										}
									}
									vctr::otherEntity new_entity;
									new_entity.entity = entity.entity;
									temp_samsites.push_back(new_entity);
								}
							}
						}

						cached_samsites = temp_samsites;
					}
					else {
						cached_samsites.clear();
						did_mounted = false;
					}
					global::float_map["samsite"] = currentTime;
				}
			}
		}

		if (std::get<bool>(ui::element["nearest_player_clipboard"].value)) {
			if (functions::get_key(std::get<KeyCode>(ui::element["nearest_player_clipboard_key"].value))) {
				if (!doing_nearest_player) {
					FovResult result = aimbot::InFov(500, true);
					if (result.saved_player) {
						if (result.saved_player->get_life_state() != 1) {
							std::string steamid = global::steam_id_64(result.saved_player->get_uid());
							std::string string_name = global::removeNonAlphabetical(functions::ws2s(result.saved_player->get_username()));
							if (string_name.length() > 15) {
								string_name = string_name.substr(0, 15);
							}
							global::CopyTextToClipboard("https://steamcommunity.com/profiles/" + steamid);
							notifications::add_notification("Copied " + functions::ws2s(result.saved_player->get_username()) + "'s steamid ", 5);
						}
						else {
							notifications::add_notification("Nearest player not alive", 5);
						}
					}
					else {
						notifications::add_notification("Failed to find nearest player", 5);
					}
				}
				doing_nearest_player = true;
			}
			else {
				doing_nearest_player = false;
			}
		}

		if (variables::local_player && variables::local_player->get_life_state() != 1) {
			if (std::get<bool>(ui::element["suicide"].value) && functions::get_key(std::get<KeyCode>(ui::element["suicide_key"].value))) {
				if ((currentTime - global::get_float("suicide")) > 25) {
					variables::lcp.movement->set_was_falling(true);
					variables::lcp.movement->set_previous_velocity(Vector3{ 0, -1000, 0 });
					variables::lcp.movement->set_ground_time(0);
					global::float_map["suicide"] = currentTime;
				}
			} else if (std::get<bool>(ui::element["nofall"].value)) {
				variables::lcp.movement->set_was_falling(true);
				variables::lcp.movement->set_previous_velocity(Vector3{});
			}

			if (std::get<bool>(ui::element["infinite_jump"].value)) {
				rust::PlayerWalkMovement* movement = variables::local_player->get_movement();
				movement->set_land_time(0);
				movement->set_jump_time(0);
				movement->set_ground_time(100000);
			}

			if (std::get<bool>(ui::element["no_collision"].value)) {
				misc::noCollision();
			}

			if (std::get<bool>(ui::element["instant_revive"].value)) {
				if (functions::get_key(std::get<KeyCode>(ui::element["instant_revive_key"].value))) {
					misc::instantRevive();
				}
			}

			if (std::get<bool>(ui::element["remove_heavyplate"].value)) {
				if (currentTime > (global::get_float("heavyplate overlay") + 2000)) {
					if (overlay) {
						overlay->set_alpha(0.f);
						set_alpha = true;
					}
					global::float_map["heavyplate overlay"] = currentTime;
				}
			}

			if (std::get<bool>(ui::element["silent_stash"].value)) {
				if (functions::get_key(std::get<KeyCode>(ui::element["silent_stash_key"].value))) {
					if (!toggle_stash) {

						for (int i = 0; i < variables::entity_draw.size(); i++) {
							rust::entity_base ent = variables::entity_draw[i];
							if (ent.name == "STASH") {
								if (((rust::StashContainer*)(ent.entity))->is_hidden()) {
									auto ent_distance = Math::Calc3D_Dist(variables::local_position, ent.position);
									if (2.7 > ent_distance) {
										variables::local_player->server_rpc(ent.entity, ("RPC_OpenLoot"));
										toggle_stash = true;
									}
								}
							}
						}
					}
				}
				else {
					toggle_stash = false;
				}
			}

			if (std::get<bool>(ui::element["fake_shoot"].value)) {
				if (functions::get_key(std::get<KeyCode>(ui::element["fake_shoot_key"].value))) {
					if ((currentTime - global::get_float("fakeshoot")) > 30) {
						misc::fakeShoot();
						global::float_map["fakeshoot"] = currentTime;
					}
				}
			}

			if (std::get<bool>(ui::element["insta_eoka"].value)) {
				misc::InstaEoka();
			}

			misc::fov_change(std::get<bool>(ui::element["fov_change"].value), std::get<float>(ui::element["fov_change_value"].value));
		}

		if (std::get<bool>(ui::element["crosshair_enable"].value)) {
			functions::fill_box(Vector4(functions::get_screen_width() / 2 - 2, functions::get_screen_height() / 2 - 2, 5, 5), Vector4(0.0, 0.0, 0.0, std::get<Vector4>(ui::element["crosshair_color"].value).w)); // Draw the black outline
			functions::fill_box(Vector4(functions::get_screen_width() / 2 - 1, functions::get_screen_height() / 2 - 1, 3, 3), std::get<Vector4>(ui::element["crosshair_color"].value)); // Draw the fillex
		}

		if (event_type == EventType::Repaint) {
			for (int i = 0; i < variables::helis.size(); i++) {
				vctr::otherEntity ent = variables::helis[i];
				if (ent.name == "MINI" && std::get<bool>(ui::element["vehicle_mini"].value) || ent.name == "SCRAP HELI" && std::get<bool>(ui::element["vehicle_scrap"].value) || ent.name == "COMBAT HELI" && std::get<bool>(ui::element["vehicle_attackmini"].value)) {
					if ((currentTime - ent.last_check) >= 500) {
						float time_difference = current;

						if (currentTime < (ent.last_check + (std::get<float>(ui::element["vehicle_heli_linger"].value) * 1000))) {
							Vector2 draw_pos;
							if (functions::WorldToScreen(ent.position, draw_pos) == true) {
								float ent_distance = Math::Calc3D_Dist(variables::local_position, ent.position);
								std::string format_distance = "[" + std::to_string(int(ent_distance)) + "m]";
								esp::render_text(Vector4{ draw_pos.x - 80.f, draw_pos.y - 5, 160, 13 }, ent.name + " " + format_distance, ent.color, std::get<bool>(ui::element["outlines"].value), 10, true, ent_distance, std::get<bool>(ui::element["fade_out"].value));
								esp::render_text(Vector4{ draw_pos.x - 80.f, draw_pos.y + 5, 160, 13 }, std::to_string(int((currentTime - ent.last_check) / 1000.0f)) + "s", ent.color, std::get<bool>(ui::element["outlines"].value), 10, true, ent_distance, std::get<bool>(ui::element["fade_out"].value));
							}
						}
					}
				}
			}

			if (std::get<bool>(ui::element["aimbot_draw"].value)) {
				if (functions::get_key(std::get<KeyCode>(ui::element["aimbot_key"].value))) {
					if (!(currentTime > (aimbot::pred_last + 50))) {
						Vector2 draw_pos;
						if (functions::WorldToScreen(aimbot::aim_pos, draw_pos) == true) {
							esp::render_text(Vector4{ draw_pos.x - 80.f, draw_pos.y - 5, 160, 13 }, "+", Vector4{ 1, 0, 0, 1 }, true, 10, true, 0, false);
						}
					}
				}
			}
			if (std::get<bool>(ui::element["raid_esp"].value)) {
				for (raidItem& item : raids::activeRaids) {
					float y_offset = 0;
					auto distancea = Math::Calc3D_Dist(variables::local_position, item.pos);

					Vector2 raid_pos;
					if (functions::WorldToScreen(item.pos, raid_pos) == false) continue;

					esp::render_text(Vector4{ raid_pos.x - 80.f, raid_pos.y - 10, 160, 13 }, item.grid + " [" + global::floatToString(distancea) + "m]", Vector4{
						std::get<Vector4>(ui::element["raid_esp_color"].value).x,
						std::get<Vector4>(ui::element["raid_esp_color"].value).y,
						std::get<Vector4>(ui::element["raid_esp_color"].value).z,
						std::get<Vector4>(ui::element["raid_esp_color"].value).w,
						}, std::get<bool>(ui::element["outlines"].value), 10, true, 0, false);

					for (raidType& explo : item.explosions) {
						esp::render_text(Vector4{ raid_pos.x - 80.f, raid_pos.y + y_offset, 160, 13 }, explo.type + " x" + global::floatToString(explo.count), Vector4{
							std::get<Vector4>(ui::element["raid_esp_color"].value).x,
							std::get<Vector4>(ui::element["raid_esp_color"].value).y,
							std::get<Vector4>(ui::element["raid_esp_color"].value).z,
							std::get<Vector4>(ui::element["raid_esp_color"].value).w,
						}, std::get<bool>(ui::element["outlines"].value), 10, true, 0, false);
						y_offset = y_offset + 10;
					}
				}
				raids::activeRaids.erase(
					std::remove_if(raids::activeRaids.begin(), raids::activeRaids.end(),
						[currentTime](const raidItem& item) {
							return currentTime > (item.time + (std::get<float>(ui::element["raid_esp_seconds"].value)*1000));
						}),
					raids::activeRaids.end()
				);
			}
			if (std::get<bool>(ui::element["aimbot_fov"].value)) {
				if (std::get<bool>(ui::element["aimbot_fovDraw"].value)) {
					functions::draw_outline_circle(functions::get_screen_width() / 2, functions::get_screen_height() / 2, aimbot::dynamic_fov, std::get<Vector4>(ui::element["aimbot_fovColor"].value));
				}
			}
			if (std::get<bool>(ui::element["radar"].value)) {
				esp::radar(std::get<bool>(ui::element["radar_teammates"].value));
			}
			if (std::get<bool>(ui::element["player_hotbar"].value)) {
				esp::do_hotbar(std::get<float>(ui::element["player_hotbar_fov"].value), std::get<float>(ui::element["player_hotbar_offset"].value));
			}
			if (std::get<bool>(ui::element["rocket_trajectory"].value)) {
				rust::Item* held_item = variables::local_player->get_held_item();
				if (held_item) {
					if (held_item && (reinterpret_cast<uintptr_t>(held_item)))
					{
						rust::PlayerEyes* local_eyes = variables::local_player->get_eyes();
						if (local_eyes) {
							misc::rocketTrajectory(local_eyes, held_item);
							//if (std::get<bool>(ui::element["grenade_trajectory"].value)) {
							//	misc::grenadeTrajectory(local_eyes, held_item);
							//}
						}
					}
				}
			}
		}

		misc::nospread(std::get<bool>(ui::element["spread_adjust"].value), std::get<float>(ui::element["spread_value"].value));
		misc::ReducedRecoil(std::get<bool>(ui::element["enable_recoil"].value));

		if (std::get<bool>(ui::element["radar_enable"].value)) {
			//radar::player_data(dropdowns::get_isSelected("Radar Options", "Ore"), std::get<float>(ui::element["radar_updaterate"].value), dropdowns::get_isSelected("Radar Options", "Shared Radar"));
		}

		if (std::get<bool>(ui::element["admin_enable"].value)) {
			if (std::get<bool>(ui::element["admin_key_enable"].value)) {
				if (functions::get_key(std::get<KeyCode>(ui::element["admin_key"].value))) {
					misc::admin_flags();
				}
			}
			else {
				misc::admin_flags();
			}
		}

		if (std::get<bool>(ui::element["admin_block_commands"].value)) {
			if (std::get<bool>(ui::element["admin_block_commands_adminonly"].value)) {
				if (variables::local_player->has_flag(4)) {
					if (disabledRunFromServer == false) {
						misc::runFromServer(false);
						disabledRunFromServer = true;
					}
				}
				else {
					if (disabledRunFromServer == true) {
						misc::runFromServer(true);
						disabledRunFromServer = false;
					}
				}
			}
			else {
				if (disabledRunFromServer == false) {
					misc::runFromServer(false);
					disabledRunFromServer = true;
				}
			}
		}
		else {
			if (disabledRunFromServer == true) {
				misc::runFromServer(true);
				disabledRunFromServer = false;
			}
		}

		if (std::get<bool>(ui::element["flyhack"].value)) {

			if (functions::get_key(std::get<KeyCode>(ui::element["flyhack_key"].value))) {
				set_flying = true;
				misc::flyhack();
			}
			else {
				if (set_flying) {
					auto player_movement = variables::local_player->get_movement();
					player_movement->flying(false);
					player_movement->set_capsule_center(0.9f);
					set_flying = false;
				}
			}
		}
		else {
			if (set_flying) {
				auto player_movement = variables::local_player->get_movement();
				player_movement->flying(false);
				player_movement->set_capsule_center(0.9f);
				set_flying = false;
			}
		}

		if (std::get<bool>(ui::element["fullbright_enable"].value)) {
			if (functions::get_key(std::get<KeyCode>(ui::element["fullbright_key"].value))) {
				if (!holding_fullbright) {
					fullbright_active = !fullbright_active; // Toggle the fullbright_active state
					holding_fullbright = true;
				}
			}
			else {
				holding_fullbright = false;
			}
			if (fullbright_active) {
				misc::fullbright();
			}
		}

		return;
	}

	static bool wakeup{ true };
	uintptr_t performaceui_hook;
	void hk_performance_ui_update(void* instance)
	{
		if (wakeup)
		{
			auto game_object = il2cpp::methods::object_new(il2cpp::init_class(xorstr_("GameObject"), xorstr_("UnityEngine")));
			functions::create(game_object, xorstr_(""));
			functions::add_component(game_object, il2cpp::type_object(xorstr_(""), xorstr_("DevControls")));
			functions::dont_destroy_on_load(game_object);
			wakeup = false;
		}

		auto static update_performance_ui = reinterpret_cast<void(*)(void* instance)>(performaceui_hook);
		SpoofReturn(update_performance_ui, instance);
	}
}