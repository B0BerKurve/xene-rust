#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {

	il2cpp::il2cppstring* DownloadHandler::GetText()
	{
		if (!this)
			return il2cpp::il2cppstring::New("Failed to get Text!");

		uintptr_t method = offsets::download_handler_get_text;
		if (!method)
		{
			return il2cpp::il2cppstring::New("Failed to get Text!");
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return il2cpp::il2cppstring::New("Failed to get Text!");
		}

		auto get_Text = reinterpret_cast<il2cpp::il2cppstring * (*)(uintptr_t)>(method_ptr);
		return SpoofReturn(get_Text, (uintptr_t)this);
	}

	UnityWebRequest* UnityWebRequest::Get(il2cpp::il2cppstring* url)
	{
		uintptr_t method = offsets::unity_web_request_get;
		if (!method)
		{
			//std::cout << "Failed to get UWR::GET method: " << method << std::endl;
			auto new_method = il2cpp::method(xorstr_("UnityWebRequest"), xorstr_("Get"), 1, "Networking");
			if (!new_method)
			{
				//std::cout << "Failed to get NEW UWR::GET method: " << new_method << std::endl;
				return nullptr;
			}
			else
			{
				method = new_method;
			}
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return nullptr;
		}

		auto get = reinterpret_cast<UnityWebRequest * (*)(il2cpp::il2cppstring*)>(method_ptr);
		return SpoofReturn(get, url);
	}

	uintptr_t UnityWebRequest::SendWebRequest()
	{
		if (!this)
			return uintptr_t();

		uintptr_t method = offsets::unity_web_request_send_web_request;
		if (!method)
		{
			return uintptr_t();
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return uintptr_t();
		}

		auto sendwebrequest = reinterpret_cast<uintptr_t(*)(uintptr_t)>(method_ptr);
		SpoofReturn(sendwebrequest, (uintptr_t)this);
	}

	bool UnityWebRequest::set_header(il2cpp::il2cppstring* name, il2cpp::il2cppstring* value)
	{
		if (!this)
			return false;

		uintptr_t method = offsets::unity_web_request_set_request_header;
		if (!method)
			return false;

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
			return false;

		auto set_request_header = reinterpret_cast<uintptr_t(*)(UnityWebRequest*, il2cpp::il2cppstring*, il2cpp::il2cppstring*)>(method_ptr);
		auto result = set_request_header(this, name, value);

		return true; // Assuming a return value of 0 indicates success
	}

	DownloadHandler* UnityWebRequest::GetDownloadHandler()
	{
		if (!this)
			return nullptr;

		uintptr_t method = offsets::unity_web_request_get_download_handler;
		if (!method)
		{
			return nullptr;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return nullptr;
		}

		auto getdownloadhandler = reinterpret_cast<DownloadHandler * (*)(uintptr_t)>(method_ptr);
		return SpoofReturn(getdownloadhandler, (uintptr_t)this);
	}
}