#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	float HackableLockedCrate::get_time() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return 0;

		uintptr_t address = offsets::crate_seconds;
		if (!address)
			return 0;

		auto timeLeft = *reinterpret_cast<float*>(entity + address);

		return timeLeft;
	}
}