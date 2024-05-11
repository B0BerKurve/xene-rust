#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	Vector3 PlayerModel::get_position()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!entity)
			return Vector3();

		return *reinterpret_cast<Vector3*>(entity + offsets::playermodel_position);
	}

	void PlayerModel::set_position(Vector3 new_pos)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!entity)
			return;

		*reinterpret_cast<Vector3*>(entity + offsets::playermodel_position) = new_pos;
	}

	Vector3 PlayerModel::get_velocity()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!entity)
			return Vector3();

		return *reinterpret_cast<Vector3*>(entity + offsets::playermodel_newVelocity);
	}

	uintptr_t PlayerModel::get_multimesh()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!entity)
			return NULL;

		return *reinterpret_cast<uintptr_t*>(entity + offsets::playermodel_multimesh);
	}

	bool PlayerModel::is_npc()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!entity)
			return bool();

		return *reinterpret_cast<bool*>(entity + 0x320);
	}

}