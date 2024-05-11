#pragma once
#include "includes.hpp"
#include "game/classes.hpp"

namespace esp
{
	void oof_arrows(Vector3 player_pos, Vector4 color);
	void do_hotbar(float fov, float offset);
	void do_skeleton(rust::BasePlayer* player, Vector4 color);
	void render_Health(rust::BasePlayer* player, rust::box_bounds bounds, bool teammate);
	void render_text(Vector4 position, std::string text, Vector4 color, bool outline, float fontsize, bool center, float distance, bool do_fade);
	void render_box(rust::BasePlayer* player, rust::box_bounds bounds, Vector4 color);
	void render_apc(rust::BaseCombatEntity* heli, Vector2 pos);
	void do_chams(rust::BasePlayer* player, Vector4 color);
	void radar(bool teammates);
	//void aimbot(rust::BasePlayer* player);
	//void PSlient(rust::BasePlayer* player);
}