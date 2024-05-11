#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	bool StashContainer::is_hidden() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return false;
		uintptr_t method = offsets::stash_hidden;
		if (!method) {
			return false;
		}
		
		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return false;
		}

		auto hiddenStash = reinterpret_cast<bool(*)(uintptr_t)>(method_ptr);
		return hiddenStash(entity);
	}

}