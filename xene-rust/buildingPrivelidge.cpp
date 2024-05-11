#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"
#include "global.h"

namespace rust {
	bool BuildingPrivelidge::is_authorized(std::vector<authList> authlist) {
		return std::any_of(authlist.begin(), authlist.end(), [](const authList& entry) {
			return entry.userid == variables::local_userid;
		});
	}

	float BuildingPrivelidge::get_upkeep() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return 0;

		uintptr_t address = offsets::tc_upkeep;
		if (!address)
			return 0;

		auto upkeep_time = *reinterpret_cast<float*>(entity + address);
		if (!upkeep_time)
			return 0;

		return upkeep_time;
	}

	std::vector<authList> BuildingPrivelidge::get_authorized()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return std::vector<authList>();

		uintptr_t address = offsets::tc_authorized; //il2cpp::value(xorstr_("BaseCombatEntity"), xorstr_("_health"));
		if (!address)
			return std::vector<authList>();

		auto authorizedPlayers_wrapper = *reinterpret_cast<uintptr_t*>(entity + address);
		if (!authorizedPlayers_wrapper)
			return std::vector<authList>();

		const auto authorizedPlayers_list = *reinterpret_cast<rust::list<PlayerNameID*>**>(authorizedPlayers_wrapper + 0x10);
		if (!authorizedPlayers_list)
			return std::vector<authList>();

		std::vector<authList> returnList;

		authorizedPlayers_list->for_each([&](PlayerNameID* authorizedPlayer, int32_t idx) {
			auto user = authorizedPlayer->get_username();
			auto id = authorizedPlayer->get_user_id();

			if (user.length() > 15) {
				user = user.substr(0, 15);
			}

			authList new_entry;
			new_entry.name = global::removeNonAlphabetical(user);
			new_entry.userid = id;
			new_entry.steam64 = global::steam_id_64(id);
			returnList.push_back(new_entry);
			//tempNpc.push_back(player);
		});

		return returnList;
	}

}