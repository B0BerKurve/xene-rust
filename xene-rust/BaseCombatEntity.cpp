#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	float BaseCombatEntity::get_max_health()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return 0.f;

		uintptr_t address = offsets::basecombatentity__maxHealth;
		if (!address)
			return 0.f;

		return *reinterpret_cast<float*>(entity + address);
	}

	int BaseCombatEntity::get_life_state()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return 1;

		uintptr_t address = offsets::basecombatentity_lifestate; //il2cpp::value(xorstr_("BaseCombatEntity"), xorstr_("lifestate"));
		if (!address)
			return 1;

		return *reinterpret_cast<int*>(entity + address);
	}

	float BaseCombatEntity::get_health()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return 0.f;

		uintptr_t address = offsets::basecombatentity__health; //il2cpp::value(xorstr_("BaseCombatEntity"), xorstr_("_health"));
		if (!address)
			return 0.f;

		return *reinterpret_cast<float*>(entity + address);
	}

}