#pragma once
#include "includes.hpp"
#include "game/classes.hpp"

namespace misc
{
	void admin_flags();
	void flyhack();
	void fov_change(bool enabled, float value);
	void runFromServer(bool set);
	void nospread(bool should_do, float value);
	void ReducedRecoil(bool should_do);
	void isflying();
	void instantRevive();
	void noCollision();
	void fakeShoot();
	void fullbright();
	void InstaEoka();
	void rocketTrajectory(rust::PlayerEyes* local_eyes, rust::Item* held_item);
	//void isflying();

}