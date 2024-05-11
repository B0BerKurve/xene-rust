#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	ItemDefinition* BaseProjectile::Magazine::get_ammotype()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return nullptr;

		return *reinterpret_cast<ItemDefinition**>(entity + 0x20);
	}

	BaseProjectile::Magazine* BaseProjectile::get_primaryMagazine()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return NULL;

		return *reinterpret_cast<BaseProjectile::Magazine**>(entity + offsets::baseprojectile_primaryMagazine);
	}

}