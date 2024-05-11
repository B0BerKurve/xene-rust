#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	bool AutoTurret::is_authorized(std::vector<authList> authlist) {
		return std::any_of(authlist.begin(), authlist.end(), [](const authList& entry) {
			return entry.userid == variables::local_userid;
		});
	}

	bool AutoTurret::is_online() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return true;

		uintptr_t method = offsets::turret_powered; 
		if (!method)
			return true;

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return 0.f;
		}

		auto onlineTurret = reinterpret_cast<bool(*)(uintptr_t)>(method_ptr);
		return onlineTurret(entity);
	}

	std::vector<authList> AutoTurret::get_authorized()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return std::vector<authList>();

		uintptr_t address = offsets::turret_authorized; 
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
			authList new_entry;
			new_entry.name = user;
			new_entry.userid = id;

			if (variables::local_userid == id) {

			}

			returnList.push_back(new_entry);
			//tempNpc.push_back(player);
		});

		return returnList;
	}

}