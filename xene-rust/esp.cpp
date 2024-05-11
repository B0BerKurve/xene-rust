#include "esp.h"
#include <map>
#include <utility>
#include <string>
#include "aimbot.h"
#include "global.h"
#include "settings.h"


std::map<std::string, double> makeHealthColor(double player_health, double max_health) {
	double r, g;
	std::map<std::string, double> color;

	// Ensure that player_health and max_health are within valid bounds
	if (player_health < 0.0) {
		player_health = 0.0;
	}
	if (player_health > max_health) {
		player_health = max_health;
	}

	// Calculate the color based on player_health and max_health
	double factor = player_health / max_health;

	if (factor >= 0.5) {
		r = (255.0 - 255.0 * (factor - 0.5) / 0.5) / 255.0;
		g = 1.0;
	}
	else {
		r = 1.0;
		g = (255.0 * factor / 0.5) / 255.0;
	}

	color["r"] = r;
	color["g"] = g;

	return color;
}

struct BoneConnection {
	BoneList from;
	BoneList to;
};

std::vector<BoneConnection> boneConnections = {
	{BoneList::head, BoneList::neck},
	{BoneList::neck, BoneList::spine4},
	{BoneList::spine4, BoneList::r_upperarm},
	{BoneList::r_upperarm, BoneList::r_forearm},
	{BoneList::r_forearm, BoneList::r_ulna},
	//{BoneList::r_ulna, BoneList::IKtarget_righthand_min},/////////
	{BoneList::spine4, BoneList::l_upperarm}, //////////
	{BoneList::l_upperarm, BoneList::l_forearm}, ////////
	{BoneList::l_forearm, BoneList::l_ulna}, ///////
	//{BoneList::l_ulna, BoneList::IKtarget_lefthand_min},////////
	{BoneList::spine4, BoneList::spine3}, ///////
	{BoneList::spine3, BoneList::spine2}, /////////
	{BoneList::spine2, BoneList::pelvis}, /////////
	{BoneList::pelvis, BoneList::r_knee},///////
	{BoneList::r_knee, BoneList::r_ankle_scale},
	{BoneList::pelvis, BoneList::l_knee},////////
	{BoneList::l_knee, BoneList::l_ankle_scale}
};

namespace esp
{
	void oof_arrows(Vector3 player_pos, Vector4 color)
	{
		Vector2 center = Vector2(variables::screen_width, variables::screen_height) / 2;

		Vector3 euler_angles = Math::to_euler_angles(variables::lcp.eyes->get_rotation());

		const Vector2 position = Vector2(variables::local_position.x - player_pos.x, variables::local_position.z - player_pos.z).normalize();

		float angle = atan2(position.x, position.y) * 57.29578f - 180.f - euler_angles.y;

		Vector2 pos_0 = Math::cos_tan_horizontal(angle, 10.f, center.x, center.y, 140);
		Vector2 pos_1 = Math::cos_tan_horizontal(angle + 2.f, 10.f, center.x, center.y, 130);
		Vector2 pos_2 = Math::cos_tan_horizontal(angle - 2.f, 10.f, center.x, center.y, 130);

		// Using the new draw_triangle function
		functions::draw_triangle(pos_0, pos_1, pos_2, color);
	}

	void do_hotbar(float fov, float offset) {
		FovResult result = aimbot::InFov(fov, true);
		double last_distance = result.last_distance;
		rust::BasePlayer* player = result.saved_player;
		bool isInFov = result.in_fov;
		if (isInFov) {
			if (player) {
				auto belt = player->get_hotbar_items();
				if (!belt.empty())
				{
					int a_width = functions::get_screen_width();
					float s_width = a_width;
					float posx = (s_width /2 - ((3*75) - 10));
					float posy = functions::get_screen_height() - offset;
					float info_y = 0;

					std::string string_name = functions::ws2s(player->get_username());
					if (string_name.length() > 15) {
						string_name = string_name.substr(0, 15);
					}

					render_text({ s_width/2 - 90, posy - 15, 160, 13 }, string_name, { 1, 1, 1, 1 }, true, 10, true, 0, false);

					for (int i = 0; i < belt.size(); i++)
					{
						static float r = 1.00f, g = 0.00f, b = 1.00f;
						
						functions::fill_box({ posx + info_y, posy, 55, 55 }, { 0.1, 0.1, 0.1, 0.2 });

						if (i > belt.size() - 1)
						{
							info_y += 75;
							continue;
						}

						auto item = belt.at(i);
						if (!item)
							return;

						if (item != NULL)
						{

							auto weapon_name = functions::ws2s(item->get_name());

							if (!weapon_name.empty())
							{
								auto sprite_the_drink = item->get_sprite();
								if (!sprite_the_drink)
									continue;

								auto texture = functions::get_texture(sprite_the_drink);
								if (!texture)
									continue;


								functions::set_draw_color({ 1, 1, 1, 1 });
								functions::draw_texture({ posx + info_y, posy, 55, 55 }, texture);
								info_y += 70;
							}
						}

					}
					info_y = 0;
				}
			}
		}
	}
	void do_skeleton(rust::BasePlayer* player, Vector4 color) {
		int loop_count = 0;
		for (const auto& connection : boneConnections) {
			Vector3 fromPos = player->get_bone_position(connection.from);
			Vector2 fromPos2D;
			if (functions::WorldToScreen(fromPos, fromPos2D)) {
				Vector3 toPos = player->get_bone_position(connection.to);
				Vector2 toPos2D;
				if (functions::WorldToScreen(toPos, toPos2D)) {
					//functions::get_screen_width() //screen width
					//functions::get_screen_height() //screen height
					if (fromPos2D.x >= 3 && fromPos2D.x <= (functions::get_screen_width() - 3)) {
						if (fromPos2D.y >= 3 && fromPos2D.y <= (functions::get_screen_height() - 3)) {


							if (toPos2D.x >= 3 && toPos2D.x <= (functions::get_screen_width() - 3)) {
								if (toPos2D.y >= 3 && toPos2D.y <= (functions::get_screen_height() - 3)) {
									functions::line(fromPos2D, toPos2D, color);
								}
							}


						}
					}
				}
			}
			loop_count = loop_count + 1;
		}
	}

	void RotatePoint(float* pointToRotate, float* centerPoint, float angle, float* ReturnTo)
	{
		angle = (float)(angle * (M_PI / (float)180));

		float cosTheta = (float)cos(angle);
		float sinTheta = (float)sin(angle);

		ReturnTo[0] = cosTheta * (pointToRotate[0] - centerPoint[0]) - sinTheta * (pointToRotate[1] - centerPoint[1]);
		ReturnTo[1] = sinTheta * (pointToRotate[0] - centerPoint[0]) + cosTheta * (pointToRotate[1] - centerPoint[1]);

		ReturnTo[0] += centerPoint[0];
		ReturnTo[1] += centerPoint[1];
	}

	void radar(bool teammates)
	{
		// Offset values
		float xOffset = 25.0f;
		float yOffset = 25.0f;

		// if (options::radar)
		{
			if (variables::local_player)
			{
				float x = xOffset, y = yOffset; // Adjust the radar position
				float resx = 200, resy = 200;
				float screenPos[2];
				float RotatedPoint[2];
				float centerx = x + (resx / 2), centery = y + (resy / 2); // Update center position
				float centerWindowPos[2] = { centerx, centery };

				functions::fill_box({ x, y, resx, resy }, { 0.2f, 0.2f, 0.2f, 0.2f });

				for (int i = 0; i < variables::players.size(); i++)
				{
					auto player_ent = variables::players[i];
					rust::BasePlayer* player = (rust::BasePlayer*)player_ent.entity;
					ULONG local_teamid = variables::local_player->get_team_id();
					rust::PlayerTeam* local_team = variables::local_player->get_team();




					bool should_do = true;
					static float r = 0.005f, g = 0.00f, b = 1.00f;

					if ((reinterpret_cast<uintptr_t>(local_team)) && (reinterpret_cast<uintptr_t>(player_ent.team)) && player_ent.teamid == local_teamid) {
						r = 0.36470588235294116; g = 0.9411764705882353, b = 0.3254901960784314;
						if (!teammates) {
							should_do = false;
						}
					}

					if (should_do) {
						screenPos[0] = variables::local_position.x - player->get_bone_position(BoneList::spine1).x;
						screenPos[1] = variables::local_position.z - player->get_bone_position(BoneList::spine1).z;
						screenPos[0] *= 1;
						screenPos[1] *= 1;
						screenPos[0] *= -1;
						screenPos[0] += centerx;
						screenPos[1] += centery;

						RotatePoint(screenPos, centerWindowPos, 360.0f - variables::local_player->get_input()->get_view_angles().y, RotatedPoint);

						if (RotatedPoint[0] < x) continue;
						if (RotatedPoint[1] < y) continue;
						if (RotatedPoint[0] > x + resx) continue;
						if (RotatedPoint[1] > y + resy) continue;

						functions::fill_box({ RotatedPoint[0] - 1, RotatedPoint[1] - 1, 4, 4 }, { 0, 0, 0, 1 });
						functions::fill_box({ RotatedPoint[0], RotatedPoint[1], 2, 2 }, { r, g, b, 1.0f });
					}
				}
				
				functions::fill_box({ centerx, centery, 2, 2 }, { 1, 0.5, 0, 1 });
			}
		}
	}
	uintptr_t shader;


	void do_chams(rust::BasePlayer* player, Vector4 color)
	{
		auto multiMesh = player->get_model()->get_multimesh();
		auto ourmesh = variables::local_player->get_model()->get_multimesh();

		if (!multiMesh)
			return;

		auto renderers = functions::get2renderers(multiMesh);
		auto ourrenders = functions::get2renderers(ourmesh);
		if (!renderers)
			return;

		for (int i = 0; i < renderers->get_size(); i++)
		{
			auto renderer = renderers->get_value(i);

			if (!renderer)
				return;

			const auto material = functions::get_material(renderer);
			if (!material)
				return;

			if (!shader)
				shader = functions::find("Hidden/Internal-Colored"); // there are two flatshader.shader and shader.shader
			//if (functions::get_shader(material) == shader)
				//return;

			functions::set_shader(material, shader);

			//functions::set_material_int(material, "_ZTest", 8);

			functions::set_color(material, "_Color", color);
			//functions::set_color(material, "_Color2", { 0.5, 0.2, 1, 1 });

			//functions::set_color(material, "ColorBehind", { 0.4392156862745098, 0.9490196078431372, 0.3843137254901961, 1 });
			//functions::set_color(material, "ColorVisible", { 0.4392156862745098, 0.9490196078431372, 0.3843137254901961, 1 });
		}
		
	}

	void render_Health(rust::BasePlayer* player, rust::box_bounds bounds, bool teammate) {
		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;
		auto half = (bounds.right - bounds.left) / 2;

		float player_health = player->get_health();
		if (player_health > 100.0f) {
			player_health = 100.0f;
		}
		std::map<std::string, double> result = makeHealthColor(player_health, 100);

		auto health = Vector4(bounds.left - 3, bounds.top + (box_height - box_height * (player_health / 100)), 2, box_height * (player_health / 100));

		if (teammate) {
			functions::fill_box(health, Vector4(0.607843, 0.262745, 1, 1));
		}
		else {
			functions::fill_box(health, Vector4(result["r"], result["g"], 0, 1));
		}
	}

	void render_apc(rust::BaseCombatEntity* heli, Vector2 pos) {
		float health = reinterpret_cast<rust::BaseCombatEntity*>(heli)->get_health();
		float maxhealth = reinterpret_cast<rust::BaseCombatEntity*>(heli)->get_max_health();
		if (health > 1) {
			// Calculate the width of the health bar as a proportion of health to maxhealth
			float health_ratio = health / maxhealth;
			int health_bar_width = static_cast<int>(70.0f * health_ratio);

			std::map<std::string, double> result = makeHealthColor(health, maxhealth);
			auto health_bar = Vector4(pos.x - (health_bar_width/2), pos.y + 12, health_bar_width, 2);
			functions::fill_box(Vector4{ health_bar.x - 1, health_bar.y - 1, health_bar.z + 3, health_bar.w + 3 }, Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1));
			functions::fill_box(health_bar, Vector4(result["r"], result["g"], 0, 1));
		}
	}

	void render_box(rust::BasePlayer* player, rust::box_bounds bounds, Vector4 color) {
		auto half = (bounds.right - bounds.left) / 2;
		float box_width = bounds.right - bounds.left;
		float box_height = bounds.bottom - bounds.top;

		functions::outline_box({
			bounds.left,
			bounds.top
			}, {
				box_width,
				box_height
			}, color, 1);
	}
	void render_text(Vector4 position, std::string text, Vector4 color, bool outline, float fontsize, bool center, float distance, bool do_fade) { //centered text
		float alpha = color.w;
		//position.x = position.x + 2.5;
		if (do_fade) {
			float scale_alpha = 0.37;

			if ( distance <= 50.0) {
				alpha = 1;
			}
			else if (distance >= 200.0) {
				alpha = scale_alpha;
			}
			else {
				alpha = scale_alpha + ((1.0 - scale_alpha) * (200.0 - distance) / (200.0 - 50.0));
			}
		}

		if (outline) {
			functions::outline(position, text, Vector4(color.x, color.y, color.z, alpha), center, fontsize);
		}
		else {
			functions::label(position, text, Vector4(color.x, color.y, color.z, alpha), center, fontsize);
		}
	}
}