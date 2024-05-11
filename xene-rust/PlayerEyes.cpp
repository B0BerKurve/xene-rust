#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	Vector3 PlayerEyes::get_position()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return Vector3();

		return *reinterpret_cast<Vector3*>(entity + 0x44);
	}
	Vector3 PlayerEyes::get_viewoffset()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return Vector3();
		return *reinterpret_cast<Vector3*>(entity + 0x38);
	}
	Vector3 PlayerEyes::get_bodyforward()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return Vector3();

		uintptr_t method = offsets::playereyes_bodyforward; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return Vector3();
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return Vector3();
		}

		auto bodyforward = reinterpret_cast<Vector3(*)(uintptr_t)>(method_ptr);
		return bodyforward(entity);
	}
	Vector3 PlayerEyes::get_bodyright()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return Vector3();

		uintptr_t method = offsets::playereyes_bodyright; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return Vector3();
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return Vector3();
		}

		auto bodyforward = reinterpret_cast<Vector3(*)(uintptr_t)>(method_ptr);
		return bodyforward(entity);
	}

	void PlayerEyes::set_body_rotation(Vector4 value)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		memory::write<Vector4>(entity + 0x44, value);
	}

	Vector4 PlayerEyes::get_body_rotation()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return Vector4();

		return *reinterpret_cast<Vector4*>(entity + 0x44);
	}

	Vector4	PlayerEyes::get_rotation() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return Vector4();

		uintptr_t method = offsets::playereyes_getrotation;
		if (!method)
			return Vector4();

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
			return Vector4();

		auto bodyright = reinterpret_cast<Vector4(*)(uintptr_t)>(method_ptr);
		return SpoofReturn(bodyright, entity);
	}
}