#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include "radar.h"
#include "game/classes.hpp"
#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include <variant>
#include <map>
#include <random>
#include "global.h"
#include "playerlist.h"

bool startsWith(const std::string& fullString, const std::string& start)
{
	// Check if 'fullString' is at least as long as 'start'
	if (fullString.length() >= start.length()) {
		// Use 'substr' to get a portion of 'fullString' with the same length as 'start'
		std::string substring = fullString.substr(0, start.length());

		// Compare the extracted substring with 'start'
		if (substring == start) {
			return true; // 'fullString' starts with 'start'
		}
	}
	return false; // 'fullString' does not start with 'start' or is shorter than 'start'
}

std::vector<std::string> splitString(const std::string& str, size_t chunkSize) {
	std::vector<std::string> chunks;
	size_t length = str.length();
	for (size_t i = 0; i < length; i += chunkSize) {
		chunks.push_back(str.substr(i, chunkSize));
	}
	return chunks;
}

std::string encrypt(const std::string& input, const std::string& key) {
	std::ostringstream os;
	for (size_t i = 0; i < input.length(); i++) {
		char encrypted_char = input[i] ^ key[i % key.length()];
		os << std::setw(2) << std::setfill('0') << std::hex << (int)encrypted_char;
	}
	return os.str();
}

std::clock_t lastExecutionTime = 0;

namespace radar
{
	//std::string radar_link = global::GenerateRandomString(20);
	//std::string uid = global::GenerateRandomString(6);
	//float last_update = global::getCurrentTimeMillis();

	//float chunksize = 0;
	//bool is_global = false;
	//std::string global_link = "";

	//void default_var() {
	//	std::string content;
	//	if (global::ReadFromAppDataFolder("last.radar", content)) {
	//		//std::string randomStr = GenerateRandomString(20);
	//		//global::WriteToTempFile(randomStr);
	//		radar::radar_link = content;
	//	}
	//	else {
	//		radar::new_link();
	//	}
	//}
	//void copy_link() {
	//	if (is_global) {
	//		global::CopyTextToClipboard("https://rflx.pw/radar/?id=" + global_link);
	//	}
	//	else {
	//		global::CopyTextToClipboard("https://rflx.pw/radar/?id=" + radar_link);
	//	}
	//	std::cout << radar_link << std::endl;
	//}
	//void new_link() {
	//	std::string randomStr = global::GenerateRandomString(20);
	//	global::WriteToAppDataFolder("last.radar", randomStr);
	//	radar::radar_link = randomStr;
	//	global::CopyTextToClipboard("https://cheating.ng/rust/radar/?id=" + radar_link);
	//}

	//void player_data(bool do_ore, float update_rate, bool do_global) {
	//	std::clock_t currentTime = global::getCurrentTimeMillis();
	//	std::clock_t elapsedMillis = currentTime - lastExecutionTime;

	//	float u_rate = update_rate;

	//	if (variables::local_player) {
	//		if (playerlist::is_connected) {
	//			if (elapsedMillis >= u_rate) {
	//				chunksize = 0;
	//				lastExecutionTime = currentTime;
	//				std::ostringstream jsonOutput;
	//				bool should_shared = false;

	//				auto team_obj = variables::local_player->get_team();
	//				ULONG local_team = variables::local_player->get_team_id();

	//				jsonOutput << "{\n";
	//				if (do_global && (reinterpret_cast<uintptr_t>(team_obj))) {
	//					auto team_leader = team_obj->get_leader();
	//					if (team_leader) {
	//						std::string r_link = global::r_string(std::to_string(team_leader) + std::to_string(local_team));
	//						global_link = r_link;
	//						jsonOutput << "\"global\":\"true\",\n";
	//						jsonOutput << "\"uid\":\"" << uid << "\",\n";
	//						jsonOutput << "\"code\":\"" << r_link << "\",\n";
	//						is_global = true;
	//					}
	//					else {
	//						jsonOutput << "\"code\":\"" << radar_link << "\",\n";
	//						is_global = false;
	//					}
	//				}
	//				else {
	//					jsonOutput << "\"code\":\"" << radar_link << "\",\n";
	//					is_global = false;
	//				}
	//				jsonOutput << "\"type\":\"player_data\",\"global_type\":\"rust_radar\",\"ip\":\"" << playerlist::ip << ":" << playerlist::port << "\",\"mapsize\":\"" << playerlist::mapsize << "\",\"custom\":\"" << (playerlist::custom ? "true" : "false") << "\",\"seed\":\"" << playerlist::seed << "\",\"ur\":\"" << std::to_string(u_rate) << "\",\n";
	//				if (reinterpret_cast<uintptr_t>(team_obj)) {
	//					jsonOutput << "\"leader\":\"" << team_obj->get_leader() << "\",\n";
	//				}
	//				jsonOutput << "\"players\":[\n";

	//				if (variables::local_player) {
	//					if (!variables::local_player->is_sleeping()) {
	//						if (variables::local_player->get_life_state() != 1) {
	//							auto name = variables::local_player->get_username();
	//							float health = variables::local_player->get_health();
	//							Vector3 vis = variables::local_player->get_model()->get_position();
	//							std::string wName = functions::ws2s(variables::local_player->get_held_item()->get_name());
	//							std::string fixed_name = global::removeNonAlphabetical(functions::ws2s(name));

	//							std::string steamid = std::to_string(variables::local_player->get_uid());

	//							jsonOutput << "{\n";
	//							jsonOutput << "\"name\":\"" << fixed_name << "\",\n";
	//							jsonOutput << "\"health\":\"" << health << "\",\n";
	//							jsonOutput << "\"x\":\"" << vis.x << "\",\n";
	//							jsonOutput << "\"y\":\"" << vis.y << "\",\n";
	//							jsonOutput << "\"z\":\"" << vis.z << "\",\n";
	//							jsonOutput << "\"weapon\":\"" << wName << "\",\n";
	//							jsonOutput << "\"steamid\":\"" << steamid << "\",\n";
	//							jsonOutput << "\"teammate\":\"" << "true" << "\",\n";
	//							if ((reinterpret_cast<uintptr_t>(team_obj))) {
	//								jsonOutput << "\"teamid\":\"" << local_team << "\",\n";
	//							}
	//							if (variables::local_player->is_knocked()) {
	//								jsonOutput << "\"knocked\":\"" << "true" << "\",\n";
	//							}

	//							jsonOutput << "\"is_local\":\"" << "true" << "\",\n";
	//							jsonOutput << "},\n";
	//						}
	//					}
	//				}

	//				for (int i = 0; i < variables::players.size(); i++) {
	//					auto player = variables::players[i];
	//					if (!player.sleeping) {
	//						if (!player.npc) {
	//							if (player.life_state != 1) {
	//								if (player.entity) {
	//									jsonOutput << "{\n";
	//									jsonOutput << "\"name\":\"" << player.name << "\",\n";
	//									jsonOutput << "\"health\":\"" << player.health << "\",\n";
	//									jsonOutput << "\"x\":\"" << player.position.x << "\",\n";
	//									jsonOutput << "\"y\":\"" << player.position.y << "\",\n";
	//									jsonOutput << "\"z\":\"" << player.position.z << "\",\n";
	//									jsonOutput << "\"weapon\":\"" << functions::ws2s(player.weapon->get_name()) << "\",\n";
	//									jsonOutput << "\"steamid\":\"" << player.userid << "\",\n";
	//									if (player.knocked) {
	//										jsonOutput << "\"knocked\":\"" << "true" << "\",\n";
	//									}
	//									if (player.teamid != 0) {
	//										jsonOutput << "\"teamid\":\"" << player.teamid << "\",\n";
	//									}
	//									if (player.teammate == true) {
	//										jsonOutput << "\"teammate\":\"" << "true" << "\",\n";

	//									}
	//									jsonOutput << "},\n";
	//								}
	//							}
	//						}
	//					}
	//				}

	//				jsonOutput << "],\n";
	//				jsonOutput << "\"ore\":[\n";
	//				if (do_ore) {
	//					for (int i = 0; i < variables::entity_draw.size(); i++) {
	//						rust::entity_base ent = variables::entity_draw[i];
	//						if (ent.name == "STONE" || ent.name == "METAL" || ent.name == "SULFUR") {
	//							jsonOutput << "    {\n";
	//							jsonOutput << "\"type\":\"" << ent.name << "\",\n";
	//							jsonOutput << "\"x\":\"" << ent.position.x << "\",\n";
	//							jsonOutput << "\"y\":\"" << ent.position.y << "\",\n";
	//							jsonOutput << "\"z\":\"" << ent.position.z << "\",\n";
	//							jsonOutput << "},\n";
	//						}
	//					}
	//				}
	//				jsonOutput << "],\n";
	//				jsonOutput << "}\n";
	//				std::string jsonOutputStr = jsonOutput.str();
	//				std::string jsonOutputStrCompressed = global::gzipCompress(jsonOutputStr);
	//				//std::string jsonOutputStrCompressed = encrypt(jsonOutputStr, "k7T3GStHxfEd4DURd5g2");

	//				//std::cout << "sending" << std::endl;

	//				if (jsonOutputStrCompressed.length() <= 2800) {
	//					rust::UnityWebRequest* request = rust::UnityWebRequest::Get(il2cpp::il2cppstring::New("https://rflx.pw/data/")); https://rflx.pw/data/
	//					request->set_header(il2cpp::il2cppstring::New("CE-XR-Format"), il2cpp::il2cppstring::New("gzip"));
	//					request->set_header(il2cpp::il2cppstring::New("data1"), il2cpp::il2cppstring::New(jsonOutputStrCompressed.c_str()));

	//					request->SendWebRequest();
	//				}
	//				else {
	//					std::string operation_number = global::GenerateRandomString(5);
	//					std::vector<std::string> chunks = splitString(jsonOutputStrCompressed, 1500);

	//					chunksize = chunks.size();

	//					rust::UnityWebRequest* request = rust::UnityWebRequest::Get(il2cpp::il2cppstring::New("https://rflx.pw/data/"));
	//					request->set_header(il2cpp::il2cppstring::New("CE-XR-Format"), il2cpp::il2cppstring::New("gzip"));
	//					float loop_count = 0;

	//					for (size_t i = 0; i < chunks.size(); ++i) {
	//						std::string chunk = chunks[i];
	//						loop_count = loop_count + 1;
	//						request->set_header(il2cpp::il2cppstring::New(("data" + global::floatToString(loop_count)).c_str()), il2cpp::il2cppstring::New(chunk.c_str()));
	//					}

	//					request->SendWebRequest();
	//				}
	//			}
	//		}
	//	}
	//}
}