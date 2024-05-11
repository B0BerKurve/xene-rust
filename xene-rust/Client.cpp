#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	rust::Client* get_client() {
		uintptr_t net_class = offsets::net_class;
		if (!net_class)
			return nullptr;

		auto net_static = *reinterpret_cast<uintptr_t*>(net_class + 0xB8);
		if (!net_static)
			return nullptr;

		auto clients = *reinterpret_cast<rust::Client**>(net_static + 0x8);

		if (!clients)
			return nullptr;

		return clients;
	}

	std::string rust::Client::get_ip() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return "unset";

		uintptr_t client_method = offsets::connectedaddress;
		if (!client_method)
			return "unset";

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(client_method);

		if (!method_ptr)
			return "unset";

		auto orig_fix = reinterpret_cast<il2cpp::il2cppstring * (*)(uintptr_t)>(method_ptr);
		auto response = SpoofReturn(orig_fix, entity);

		if (!response)
			return "unset";

		auto ip_string = functions::ws2s(response->buffer);

		if (3 > ip_string.length())
			return "unset";

		return functions::ws2s(response->buffer);
	}

	int rust::Client::get_port() {
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return 0;

		uintptr_t client_method = offsets::port;
		if (!client_method)
			return 0;

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(client_method);

		if (!method_ptr)
			return 0;

		auto orig_fix = reinterpret_cast<int(*)(uintptr_t)>(method_ptr);
		auto response = SpoofReturn(orig_fix, entity);

		if (!response)
			return 0;

		return response;
	}
}