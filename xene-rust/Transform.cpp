#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	Vector3 Transform::get_position()
	{
		uintptr_t transform = reinterpret_cast<uintptr_t>(this);
		if (!(transform))
			return Vector3();

		uintptr_t method = offsets::transform_get_position;
		if (!(method))
			return Vector3();

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
			return Vector3();

		auto get_transform_position = reinterpret_cast<Vector3(*)(uintptr_t transform)>(method_ptr);
		return get_transform_position(transform);
	}

	void Transform::set_position(Vector3 value)
	{
		uintptr_t transform = reinterpret_cast<uintptr_t>(this);
		if (!(transform))
			return;

		uintptr_t method = offsets::transform_set_position; //il2cpp::method(xorstr_("Transform"), xorstr_("get_position"), 0, xorstr_("UnityEngine"));
		if (!(method))
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return;
		}

		auto set_transform_position = reinterpret_cast<void(*)(uintptr_t transform, Vector3 value)>(method_ptr);
		SpoofReturn(set_transform_position, transform, value);
	}
}