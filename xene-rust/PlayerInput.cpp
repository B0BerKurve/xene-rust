#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	Vector2 PlayerInput::get_view_angles()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return Vector2();

		uintptr_t address = offsets::playerinput_bodyAngles; //il2cpp::value(xorstr_("PlayerInput"), xorstr_("bodyAngles"));
		if (!address)
			return Vector2();

		return *reinterpret_cast<Vector2*>(entity + address);
	}

	void PlayerInput::set_view_angles(Vector2 angle)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		uintptr_t address = offsets::playerinput_bodyAngles; //il2cpp::value(xorstr_("PlayerInput"), xorstr_("bodyAngles"));
		if (!address)
			return;

		memory::write<Vector2>(entity + address, angle);
	}
}