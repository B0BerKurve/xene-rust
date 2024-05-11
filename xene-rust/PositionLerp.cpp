#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	Vector3 PositionLerp::GetEstimatedVelocity() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return Vector3();
		}

		uintptr_t method = offsets::download_handler_get_text;
		if (!method)
		{
			return Vector3();
		}
	}
}