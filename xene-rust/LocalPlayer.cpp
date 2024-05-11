#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	BasePlayer* LocalPlayer::Entity()
	{
		static auto clazz = il2cpp::init_class(xorstr_("LocalPlayer"));
		if (!(clazz))
			return nullptr;

		uintptr_t static_fields = *reinterpret_cast<uintptr_t*>(clazz + 0xB8);
		if (!(static_fields))
			return nullptr;

		return *reinterpret_cast<BasePlayer**>(static_fields);
	}

	void LocalPlayer::ConsoleMessage(const char* message)
	{
		uintptr_t method = offsets::baseplayer_consolemessage; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto consoleMessage = reinterpret_cast<void(*)(uintptr_t, il2cpp::il2cppstring*)>(method_ptr);
		uintptr_t blank = NULL;
		SpoofReturn(consoleMessage, blank, il2cpp::il2cppstring::New(message));
	}


}