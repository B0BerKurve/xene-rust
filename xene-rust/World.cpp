#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	rust::World* get_world() {
		uintptr_t net_class = offsets::world_class;
		if (!net_class)
			return nullptr;

		auto world_static = *reinterpret_cast<rust::World**>(net_class + 0xB8);
		if (!world_static)
			return nullptr;

		return world_static;
	}

	std::string rust::World::get_url() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return "";

		auto url = *reinterpret_cast<il2cpp::il2cppstring**>(entity + 0x10);

		if (!url)
			return "";

		auto url_string = functions::ws2s(url->buffer);

		if (3 > url_string.length())
			return "";

		return url_string;

		//return *reinterpret_cast<unsigned int*>(entity + 0x4);
	}

	unsigned int rust::World::get_mapsize() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return 0;

		return *reinterpret_cast<unsigned int*>(entity + 0x4);
	}

	unsigned int rust::World::get_seed() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return 0;

		return *reinterpret_cast<unsigned int*>(entity + 0x0);
	}

	bool rust::World::is_custom() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return false;

		auto map_url = *reinterpret_cast<il2cpp::il2cppstring**>(entity + 0x10);

		if (!map_url)
			return false;

		auto map_string = functions::ws2s(map_url->buffer);
		return (map_string.length() > 2);
	}
}