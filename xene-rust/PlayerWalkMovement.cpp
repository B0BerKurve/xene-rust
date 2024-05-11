#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	void PlayerWalkMovement::flying(bool value)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		uintptr_t address = offsets::playerwalkmovement_flying; //il2cpp::value(xorstr_("BaseCombatEntity"), xorstr_("lifestate"));
		if (!address)
			return;

		*reinterpret_cast<bool*>(entity + address) = value;
	}

	void PlayerWalkMovement::set_was_falling(bool value) {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return;

		uintptr_t address = offsets::playerwalkmovement_wasFalling;
		if (!address)
			return;

		*reinterpret_cast<bool*>(entity + address) = value;
	}
	void PlayerWalkMovement::set_previous_velocity(Vector3 value) {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return;

		uintptr_t address = offsets::playerwalkmovement_previousVelocity;
		if (!address)
			return;

		*reinterpret_cast<Vector3*>(entity + address) = value;
	}

	void PlayerWalkMovement::set_ground_angle(float angle)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		uintptr_t address = offsets::playerwalkmovement_groundAngle; //il2cpp::value(xorstr_("PlayerWalkMovement"), xorstr_("groundAngle"));
		if (!address)
			return;

		memory::write<float>(entity + address, angle);
	}

	void PlayerWalkMovement::set_ground_angle_new(float angle)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		uintptr_t address = offsets::playerwalkmovement_groundAngleNew; //il2cpp::value(xorstr_("PlayerWalkMovement"), xorstr_("groundAngleNew"));
		if (!address)
			return;

		memory::write<float>(entity + address, angle);
	}

	void PlayerWalkMovement::set_ground_time(float time)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		uintptr_t address = offsets::playerwalkmovement_groundTime; //il2cpp::value(xorstr_("PlayerWalkMovement"), xorstr_("groundTime"));
		if (!address)
			return;

		memory::write<float>(entity + address, time);
	}

	void PlayerWalkMovement::set_jump_time(float time)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		uintptr_t address = offsets::playerwalkmovement_jumpTime; //il2cpp::value(xorstr_("PlayerWalkMovement"), xorstr_("jumpTime"));
		if (!address)
			return;

		memory::write<float>(entity + address, time);
	}

	void PlayerWalkMovement::set_land_time(float time)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		uintptr_t address = offsets::playerwalkmovement_landTime; //il2cpp::value(xorstr_("PlayerWalkMovement"), xorstr_("landTime"));
		if (!address)
			return;

		memory::write<float>(entity + address, time);
	}

	void PlayerWalkMovement::set_targetmovement(Vector3 new_val)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		memory::write<Vector3>(entity + 0x34, new_val);
	}

	void PlayerWalkMovement::set_capsule_center(float angle) {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return;

		uintptr_t address = offsets::playerwalkmovement_capsuleCenter;
		if (!address)
			return;

		*reinterpret_cast<float*>(entity + address) = angle;
	}

}