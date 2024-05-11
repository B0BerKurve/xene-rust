#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {

	bool BuildingBlock::can_afford(int grade, BasePlayer* player, int buildingskin)
	{
		uintptr_t method = offsets::buildingblock_canaffordupgrade; //il2cpp::method(xorstr_("Input"), xorstr_("GetKey"), 1, xorstr_("UnityEngine"));
		if (!method)
		{
			return false;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return false;
		}

		auto CanAffordUpgrade = reinterpret_cast<bool(*)(uintptr_t, int, int, uintptr_t)>(method_ptr);
		return SpoofReturn(CanAffordUpgrade, reinterpret_cast<uintptr_t>(this), grade, buildingskin, (uintptr_t)player);
	}

	bool BuildingBlock::can_upgrade(int grade, BasePlayer* player, int buildingskin)
	{
		uintptr_t method = offsets::buildingblock_canchangetograde; //il2cpp::method(xorstr_("Input"), xorstr_("GetKey"), 1, xorstr_("UnityEngine"));
		if (!method)
		{
			std::cout << "method1 failed" << std::endl;
			return false;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			std::cout << "method2 failed" << std::endl;
			return false;
		}

		auto CanChangeToGrade = reinterpret_cast<bool(*)(uintptr_t, int, int, uintptr_t)>(method_ptr);
		return SpoofReturn(CanChangeToGrade, reinterpret_cast<uintptr_t>(this), grade, buildingskin,(uintptr_t)player);
	}

	void BuildingBlock::upgrade(int grade, BasePlayer* player, int buildingskin)
	{
		uintptr_t method = offsets::buildingblock_upgradetograde;
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto UpgradeToGrade = reinterpret_cast<void(*)(uintptr_t, int, int, uintptr_t)>(method_ptr);
		SpoofReturn(UpgradeToGrade, reinterpret_cast<uintptr_t>(this), grade, buildingskin,(uintptr_t)player);
	}

	int BuildingBlock::get_grade()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return -1;

		uintptr_t address = offsets::buildingblock_grade;
		if (!address)
		{
			return -1;
		}

		return *reinterpret_cast<bool*>(entity + address);
	}
}