#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	std::wstring BaseEntity::get_name()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return std::wstring();
		}

		uintptr_t address = offsets::baseentity__name; //il2cpp::value(xorstr_("BaseEntity"), xorstr_("_name"));
		if (!address)
			return std::wstring();

		il2cpp::il2cppstring* entity_name = *reinterpret_cast<il2cpp::il2cppstring**>(entity + address);
		if (!(reinterpret_cast<uintptr_t>(entity_name)))
			return std::wstring();

		return entity_name->buffer;
	}

	std::string BaseEntity::get_prefab_name()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return std::string();
		}

		auto object_name_ptr = *reinterpret_cast<uintptr_t*>(entity + 0x60);
		//std::cout << "[" << __FUNCTION__ << "] object_name_ptr: 0x" << std::hex << object_name_ptr << std::endl;
		if (!(object_name_ptr))
		{
			return std::string();
		}

		return functions::read_ascii(object_name_ptr);
	}

	Vector3 BaseEntity::get_visual_position(uintptr_t object_class)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return Vector3();

		uintptr_t player_visual = *reinterpret_cast<uintptr_t*>(object_class + 0x8);
		if (!(player_visual))
			return Vector3();

		uintptr_t visual_state = *reinterpret_cast<uintptr_t*>(player_visual + 0x38);
		if (!(visual_state))
			return Vector3();

		return *reinterpret_cast<Vector3*>(visual_state + 0x90);
	}

	Transform* BaseEntity::get_transform()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return nullptr;

		uintptr_t method = offsets::component_get_transform; //il2cpp::method(xorstr_("Component"), xorstr_("get_transform"), 0, xorstr_("UnityEngine"));
		if (!(method))
		{
			return nullptr;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return nullptr;
		}

		auto get_transform = reinterpret_cast<Transform * (*)(uintptr_t entity)>(method_ptr);
		return SpoofReturn(get_transform, entity);
	}

	void BaseEntity::server_rpc(const char* rpc)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		uintptr_t method = offsets::baseentity_serverrpc_1arg; //il2cpp::method(xorstr_("Input"), xorstr_("GetKey"), 1, xorstr_("UnityEngine"));
		if (!(method))
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return;
		}

		auto ServerRPC = reinterpret_cast<void (*)(uintptr_t, il2cpp::il2cppstring*)>(method_ptr);
		return SpoofReturn(ServerRPC, entity, il2cpp::il2cppstring::New(rpc));
	}

	void BaseEntity::server_rpc(uintptr_t entity, const char* rpc)
	{
		uintptr_t method = offsets::baseentity_serverrpc_1arg; //il2cpp::method(xorstr_("Input"), xorstr_("GetKey"), 1, xorstr_("UnityEngine"));
		if (!(method))
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return;
		}

		auto ServerRPC = reinterpret_cast<void (*)(uintptr_t, il2cpp::il2cppstring*)>(method_ptr);
		return SpoofReturn(ServerRPC, entity, il2cpp::il2cppstring::New(rpc));
	}

	void BasePlayer::server_rpcs(uintptr_t entity, const char* rpc, const char* string, bool value)
	{
		uintptr_t method = offsets::baseentity_serverrpc_4arg;
		if (!method)
			return;

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
			return;

		auto ServerRPC = reinterpret_cast<void (*)(uintptr_t, il2cpp::il2cppstring*, il2cpp::il2cppstring*, bool)>(method_ptr);
		SpoofReturn(ServerRPC, entity, il2cpp::il2cppstring::New(rpc), il2cpp::il2cppstring::New(string), value);
	}

	void BaseEntity::send_signal_broadcast(int signal, const char* arg)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return;

		uintptr_t method = offsets::baseentity_sendsignalbroadcast;
		if (!method)
			return;

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
			return;

		auto ServerRPC = reinterpret_cast<void (*)(uintptr_t, int, il2cpp::il2cppstring*)>(method_ptr);
		return SpoofReturn(ServerRPC, entity, signal, il2cpp::il2cppstring::New(arg));
	}
}