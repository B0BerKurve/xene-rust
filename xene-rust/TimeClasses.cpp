#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	void TOD_NightParameters::set_light_intensity(float intensity)
	{
		if (!this)
			return;

		*reinterpret_cast<float*>(this + offsets::tod_nightparameters_lightintensity) = intensity;
	}

	void TOD_NightParameters::set_reflection_multiplier(float multiplier)
	{
		if (!this)
			return;

		*reinterpret_cast<float*>(this + offsets::tod_nightparameters_reflectionmultiplier) = multiplier;
	}

	void TOD_NightParameters::set_ambient_multiplier(float multiplier)
	{
		if (!this)
			return;

		*reinterpret_cast<float*>(this + offsets::tod_nightparameters_ambientmultiplier) = multiplier;
	}

	void TOD_DayParameters::set_ambient_multiplier(float multiplier)
	{
		if (!this)
			return;

		*reinterpret_cast<float*>(this + offsets::tod_dayparameters_ambientmultiplier) = multiplier;
	}

	TOD_Sky* TOD_Sky::get_instance()
	{
		uintptr_t method = offsets::tod_sky_get_instance;
		if (!method)
		{
			return nullptr;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return nullptr;
		}

		auto get_instance = reinterpret_cast<TOD_Sky * (*)()>(method_ptr);
		SpoofReturn(get_instance);
	}

	TOD_NightParameters* TOD_Sky::get_night()
	{
		if (!this)
			return nullptr;

		return *reinterpret_cast<TOD_NightParameters**>(this + offsets::tod_nightparameters);
	}

	TOD_DayParameters* TOD_Sky::get_day()
	{
		if (!this)
			return nullptr;

		return *reinterpret_cast<TOD_DayParameters**>(this + offsets::tod_dayparameters);
	}
}