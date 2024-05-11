#include "misc.hpp"
#include "config.h"
#include "settings.h"
#include "./game/offsets.hpp"
#include "esp.h"
#include "utils/memory.hpp"

std::vector<std::string> weaponArray;

namespace misc
{
	double previousYaw = 0;
	Vector3 camVelocity(0, 0, 0);
	Vector3 targetmovement(0, 0, 0);

	Vector3 forward(0, 0, 0);
	Vector3 right(0, 0, 0);
	Vector3 up(0, 0, 0);
	bool camFlyToLook = true;
	float camSpeed = 0.00015f;
	float camSpeedMultiplier = 5;
	bool camFast = false;
	float camDrag = 0.99f;
	std::clock_t interactiveLast = global::getCurrentTimeMillis();

	float last_fov = 90;
	float og_fov = 90;
	bool set_fov = false;
	std::vector<vctr::otherEntity> nospread_weapons;
	float last_nospread = 8888;

	void fov_change(bool enabled, float value)
	{
		if (enabled) {
			if (!set_fov) {
				auto GraphicsStaticField = *reinterpret_cast<uintptr_t*>(offsets::graphics_convar + 0xB8);
				og_fov = memory::read<float>(GraphicsStaticField + 0x18);
				memory::write<float>(GraphicsStaticField + 0x18, value);
				last_fov = value;
				set_fov = true;
			}
			else {
				if (value != last_fov) {
					auto GraphicsStaticField = *reinterpret_cast<uintptr_t*>(offsets::graphics_convar + 0xB8);
					memory::write<float>(GraphicsStaticField + 0x18, value);
					last_fov = value;
				}
			}
		}
		else {
			if (set_fov) {
				auto GraphicsStaticField = *reinterpret_cast<uintptr_t*>(offsets::graphics_convar + 0xB8);
				memory::write<float>(GraphicsStaticField + 0x18, og_fov);
				set_fov = false;
			}
		}
	}

	void admin_flags()
	{
		variables::local_player->add_flag(4);
	}

	void flyhack()
	{
		auto player_movement = variables::local_player->get_movement();
		player_movement->flying(true);
		Vector3 mvm = {};
		Vector4 rotation = variables::local_player->get_eyes()->get_body_rotation();
		float speed = 415;

		if (!rotation.is_zero()) {
			const Vector3 vecright = Vector3(1.f, 0.f, 0.f);
			const Vector3 vecforward = Vector3(0.f, 0.f, 1.f);
			const Vector3 vecup = Vector3(0.f, 1.f, 0.f);


			if (functions::get_key(KeyCode::W))
				mvm += Math::quatmult(&vecforward, &rotation);
			if (functions::get_key(KeyCode::S))
				mvm -= Math::quatmult(&vecforward, &rotation);
			if (functions::get_key(KeyCode::A))
				mvm -= Math::quatmult(&vecright, &rotation);
			if (functions::get_key(KeyCode::D))
				mvm += Math::quatmult(&vecright, &rotation);
			if (functions::get_key(KeyCode::Space))
				mvm.y += 1.f;
			//if (functions::get_key(KeyCode::LeftShift))
				//mvm.y -= 1.f;
		}

		if (functions::get_key(KeyCode::LeftControl)) {
			speed = 235;
		} else if(functions::get_key(KeyCode::LeftShift)) {
			speed = 675;
		}

		player_movement->set_targetmovement(mvm * (speed /100.f));
		player_movement->set_capsule_center(-1000.f);
	}

	void runFromServer(bool set)
	{
		auto ConsoleSystemIndex = offsets::consolesystem_index;
		if (ConsoleSystemIndex) {
			auto ConsoleSystemIndexStaticField = *reinterpret_cast<uintptr_t*>(ConsoleSystemIndex + 0xB8);
			if (ConsoleSystemIndexStaticField)
			{
				auto BackingField = *reinterpret_cast<uintptr_t*>(ConsoleSystemIndexStaticField + 0x10);
				if (BackingField) {
					auto CommandCount = *(uint32_t*)(BackingField + 0x18);
					for (uint32_t x = 0; x < CommandCount; x++)
					{
						auto Command = *reinterpret_cast<uintptr_t*>(BackingField + 0x20 + (x * 0x8));
						if (!Command) {
							continue;
						}

						auto NameObject = *(uintptr_t*)(Command + 0x10);
						if (!NameObject) {
							continue;
						}

						auto CommandName = std::wstring((const wchar_t*)(NameObject + 0x14));
						if (CommandName == xorstr_(L"noclip") || (CommandName == xorstr_(L"debugcamera") || CommandName == xorstr_(L"camlookspeed") || CommandName == xorstr_(L"camspeed") || CommandName == xorstr_(L"debug.debugcamera"))) {
							*(bool*)(Command + 0x58) = set; // AllowRunFromServer
							//printf("Command %i %S 0x%p\n", x, CommandName.c_str(), Command);
						}
					}
				}
			}
		}
	}

	void rocketTrajectory(rust::PlayerEyes* local_eyes, rust::Item* held_item) {
		std::string item_name = functions::ws2s(held_item->get_name());
		if (item_name == "rocket.launcher") {
			Vector3 eyePos = variables::local_player->get_bone_position(BoneList::neck);
			Vector3 bodyForward = local_eyes->get_bodyforward();
			float rocketSpeed = 18.0f;
			float gravityModifier = 0.25f;//0.25f;
			float velocityScale = 1.0f;
			float step = 0.1f;
			int maxSteps = 300;

			auto held = held_item->get_held_entity();
			if (held) {
				rust::BaseProjectile* base_projectile = reinterpret_cast<rust::BaseProjectile*>(held);
				if (base_projectile) {
					rust::BaseProjectile::Magazine* magazine = base_projectile->get_primaryMagazine();
					if (magazine) {
						const auto ammo_definition = *reinterpret_cast<uintptr_t*>((uintptr_t)magazine + 0x20);
						if (ammo_definition) {
							const auto ammo_id = *reinterpret_cast<int32_t*>(ammo_definition + 0x18);
							if (ammo_id == -1841918730) {
								rocketSpeed = 40.f;
								gravityModifier = 0.05f;
							}
						}
					}
				}
			}

			Vector3 currentPos = eyePos + (bodyForward * rocketSpeed) * velocityScale * 0.3f;  // Assuming step is 0.3f
			Vector3 currentVelocity = (bodyForward * rocketSpeed) * velocityScale;
			Vector3 gravity{ 0.00f, -9.81f, 0.00f };
			Vector3 endPos;
			

			for (int i = 0; i < maxSteps; i++) {
				currentVelocity += gravity * gravityModifier * step;
				currentPos = currentPos + currentVelocity * step;

				if (variables::local_player->is_visible(currentPos)) {
					endPos = currentPos;
				}
				else {
					Vector2 draw_pos;
					if (endPos.x != 0 && endPos.y != 0 && endPos.z != 0) {
						if (functions::WorldToScreen(endPos, draw_pos) == true) {
							esp::render_text(Vector4{ draw_pos.x - 80.f, draw_pos.y - 7, 160, 13 }, "+", Vector4{ 0.09019607843137255, 0.7215686274509804, 0.9529411764705882, 1 }, true, 15, true, 0, false);
						}
					}
					break;
				}
			}
			
		}
	}
	void nospread(bool should_do, float value)
	{
		if (variables::local_player) {
			if (variables::lcp.weapon->is_gun()) {
				if (should_do) {
					//variables::lcp.weapon
					float spr = value / 100.f;
					variables::lcp.weapon->set_aim_cone(spr);
					variables::lcp.weapon->set_aim_cone_curve_scale(spr);
					variables::lcp.weapon->set_aim_cone_penalty_max(spr);
					variables::lcp.weapon->set_aim_cone_penalty_per_shot(spr);
					variables::lcp.weapon->set_aim_sway(spr);
					variables::lcp.weapon->set_aim_sway_speed(spr);

					std::ptrdiff_t does_exist = functions::get_entity_index_in_list(nospread_weapons, (uintptr_t)variables::lcp.weapon);
					if (does_exist == -1) {
						vctr::otherEntity new_entity;
						new_entity.entity = (uintptr_t)variables::lcp.weapon;
						nospread_weapons.push_back(new_entity);
						//std::cout << "set spread: ";
					}
				}
				else {
					if (!nospread_weapons.empty()) {
						std::ptrdiff_t does_exist = functions::get_entity_index_in_list(nospread_weapons, (uintptr_t)variables::lcp.weapon);
						if (does_exist != -1) {
							float spr = 100.f;
							variables::lcp.weapon->set_aim_cone(spr);
							variables::lcp.weapon->set_aim_cone_curve_scale(spr);
							variables::lcp.weapon->set_aim_cone_penalty_max(spr);
							variables::lcp.weapon->set_aim_cone_penalty_per_shot(spr);
							variables::lcp.weapon->set_aim_sway(spr);
							variables::lcp.weapon->set_aim_sway_speed(spr);
							nospread_weapons.erase(nospread_weapons.begin() + does_exist);
							//std::cout << "reset spread: ";
						}
					}
				}
			}
		}
	}

	void ReducedRecoil(bool should_do)
	{
		if (variables::local_player) {
			rust::Item* held_item = variables::local_player->get_held_item();
			if (held_item && (reinterpret_cast<uintptr_t>(held_item)))
			{
				if (held_item->is_gun())
				{
					std::string heldWeapon = functions::ws2s(held_item->get_name());
					float default_recoil = 100;
					float selected_recoil = 100;

					if (heldWeapon == "rifle.ak") {
						default_recoil = 6.0;
						selected_recoil = std::get<float>(ui::element["ak_recoil"].value) / 100;
					}
					else if (heldWeapon == "smg.2") {
						default_recoil = 3.0;
						selected_recoil = std::get<float>(ui::element["custom_recoil"].value) / 100;
					}
					else if (heldWeapon == "hmlmg") {
						default_recoil = 6.5;
						selected_recoil = std::get<float>(ui::element["hmlmg_recoil"].value) / 100;
					}
					else if (heldWeapon == "rifle.lr300") {
						default_recoil = 4.0;
						selected_recoil = std::get<float>(ui::element["lr300_recoil"].value) / 100;
					}
					else if (heldWeapon == "pistol.m92") {
						default_recoil = 9.0;
						selected_recoil = std::get<float>(ui::element["m92_recoil"].value) / 100;
					}
					else if (heldWeapon == "rifle.m39") {
						default_recoil = 6.5;
						selected_recoil = std::get<float>(ui::element["m39_recoil"].value) / 100;
					}
					else if (heldWeapon == "lmg.m249") {
						default_recoil = 6.3;
						selected_recoil = std::get<float>(ui::element["m249_recoil"].value) / 100;
					}
					else if (heldWeapon == "smg.mp5") {
						default_recoil = 4.0;
						selected_recoil = std::get<float>(ui::element["mp5_recoil"].value) / 100;
					}
					else if (heldWeapon == "pistol.python") {
						default_recoil = 18.0;
						selected_recoil = std::get<float>(ui::element["python_recoil"].value);
					}
					else if (heldWeapon == "pistol.revolver") { //rifle.semiauto
						default_recoil = 6.5;
						selected_recoil = std::get<float>(ui::element["revolver_recoil"].value);
					}
					else if (heldWeapon == "rifle.semiauto") {
						default_recoil = 3.5;
						selected_recoil = std::get<float>(ui::element["sar_recoil"].value) / 100;
					}
					else if (heldWeapon == "smg.thompson") {
						default_recoil = 3.0;
						selected_recoil = std::get<float>(ui::element["thompson_recoil"].value) / 100;
					}
					if (default_recoil != 100) {
						if (should_do) {
							held_item->recoilControl(selected_recoil);
							if (std::find(weaponArray.begin(), weaponArray.end(), heldWeapon) == weaponArray.end()) {
								weaponArray.push_back(heldWeapon);
								//std::cout << "added recoil: " << heldWeapon;
							}
						}
						else {
							if (!weaponArray.empty()) {
								auto found = std::find(weaponArray.begin(), weaponArray.end(), heldWeapon);
								if (found != weaponArray.end()) {
									held_item->recoilControl(default_recoil);
									weaponArray.erase(found);
									//std::cout << "fixed recoil: " << heldWeapon;
								}
							}
						}
					}
					else {
						if (should_do) {
							//std::cout << "cant do gun: " << heldWeapon;
						}
					}
				}
			}
		}
	}
	void fullbright()
	{
		rust::TOD_Sky* skyInstance = rust::TOD_Sky::get_instance();
		if (skyInstance) {
			rust::TOD_NightParameters* nightParams = skyInstance->get_night();
			rust::TOD_DayParameters* dayParams = skyInstance->get_day();

			if (nightParams) {
				nightParams->set_ambient_multiplier(4.1f);
				nightParams->set_light_intensity(25.f);
			}
			if (dayParams) {
				dayParams->set_ambient_multiplier(1.0f);
			}
		}
	}
	void fakeShoot()
	{
		variables::local_player->get_held_item()->fakefire();
	}
	void instantRevive()
	{
		for (int i = 0; i < variables::players.size(); i++) {
			auto player = variables::players[i];

			rust::PlayerTeam* local_team = variables::local_player->get_team();
			ULONG local_teamid = variables::local_player->get_team_id();

			if ((reinterpret_cast<uintptr_t>(local_team))) {
				if (player.teamid != 0) {
					if (player.teamid == local_teamid) {
						auto distancea = Math::Calc3D_Dist(variables::local_position, ((rust::BasePlayer*)player.entity)->get_bone_position(BoneList::BoobCensor));
						if (distancea < 4.0f)
						{
							if (player.knocked)
							{
								//std::cout << "sending rpc" << std::endl;
								variables::local_player->server_rpc(player.entity, ("RPC_Assist"));
							}
							else {
								//std::cout << "2" << std::endl;
							}
						}
						else {
							//std::cout << "1" << std::endl;
						}
					}
					else {
						//std::cout << "3" << std::endl;
					}
				}
				else {
					//std::cout << "4" << std::endl;
				}
			}
			else {
				//std::cout << "5" << std::endl;
			}
		}
	}

	void noCollision()
	{
		functions::ignore_layer_collision(layer::PlayerMovement, layer::Tree, 1);
		functions::ignore_layer_collision(layer::PlayerMovement, layer::AI, 1);
		functions::ignore_layer_collision(layer::Game_Trace, layer::TransparentFX, 1);
		functions::ignore_layer_collision(layer::Game_Trace, layer::Transparent, 1);
	}
	void isflying()
	{
		variables::local_player->get_modelstate()->set_flying(false);
	}
	void InstaEoka()
	{
		variables::local_player->get_held_item()->set_always_eoka(-1.0f);
	}
}