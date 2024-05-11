#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	void ModelState::set_flying(bool value)
	{
		/*uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return;

		uintptr_t method = offsets::modelstate_set_isflying; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto isFlying = reinterpret_cast<void(*)(uintptr_t, bool)>(method_ptr);
		SpoofReturn(isFlying, entity, value);*/
	}

	void ModelState::remove_flag(int flag)
	{
		int flags = *reinterpret_cast<int*>((uintptr_t)this + 0x24);

		flags &= ~flag;

		*reinterpret_cast<int*>((uintptr_t)this + 0x24) = flags;
	}

	void ModelState::add_flag(int flag)
	{
		int flags = *reinterpret_cast<int*>((uintptr_t)this + 0x24);

		*reinterpret_cast<int*>((uintptr_t)this + 0x24) = flags |= (int)flag;
	}

	bool ModelState::has_flag(int flag)
	{
		int flags = *reinterpret_cast<int*>((uintptr_t)this + 0x24);
		return (flags & flag);
	}



}