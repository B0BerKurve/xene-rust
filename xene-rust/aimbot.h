#pragma once
#include "game/classes.hpp"
struct FovResult {
	double last_distance;
	rust::BasePlayer* saved_player;
	Vector3 head_pos;
	bool in_fov;
};
namespace aimbot {
	extern float dynamic_fov;
	extern Vector3 aim_pos;
	extern std::clock_t pred_last;
	FovResult InFov(float fov, bool ignore);
	rust::BasePlayer* getBestPlayerByFov();
	int getClosestBoneToCrosshair(rust::BasePlayer* player);
	void run(bool silent);
}