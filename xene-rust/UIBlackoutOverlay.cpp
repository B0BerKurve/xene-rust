#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	UIBlackoutOverlay* get_overlay(int overlay) {
		uintptr_t method = offsets::blackout_get;
		if (!method)
			return nullptr;

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);

		if (!method_ptr)
			return nullptr;

		auto blackoutoverlay = reinterpret_cast<UIBlackoutOverlay*(*)(int)>(method_ptr);
		return SpoofReturn(blackoutoverlay, (int)overlay);
	}
	void UIBlackoutOverlay::set_alpha(float value) {
		uintptr_t overlay = reinterpret_cast<uintptr_t>(this);
		if (!(overlay))
			return;

		uintptr_t method = offsets::blackout_set;
		if (!method)
			return;

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);

		if (!method_ptr)
			return;

		auto set_alpha = reinterpret_cast<void(*)(uintptr_t, float)>(method_ptr);
		SpoofReturn(set_alpha, overlay, value);
	}
}