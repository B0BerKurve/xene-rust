#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	bool SamSite::is_online() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return true;

		uintptr_t method = offsets::samsite_ispowered;
		if (!method)
			return true;

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
			return false;

		auto onlineSam = reinterpret_cast<bool(*)(uintptr_t)>(method_ptr);
		return onlineSam(entity);
	}
}