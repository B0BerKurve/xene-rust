#include "classes.hpp"
#include "il2cpp.hpp"
#include "offsets.hpp"
#include "../utils/memory.hpp"
#include "../utils/returnspoofer.hpp"
#include <map>

auto gameAssembly = LI_MODULE(("GameAssembly.dll")).get<uintptr_t>(); //GetModuleHandleA(xorstr_("GameAssembly.dll"));

#define ProcAddr(func) GetProcAddress((HMODULE)gameAssembly, func)

template<typename T, typename... Args>
inline T call(const char* func, Args... args) {
	return reinterpret_cast<T(__fastcall*)(Args...)>(ProcAddr(func))(args...);
}

bool is_valid(uintptr_t entity)
{
	if (!entity)
	{
		return false;
	}

	return true;
}

il2cpp::il2cppstring* il2cpp::il2cppstring::New(const char* str)
{
	//return call<il2cpp::il2cppstring*, const char*>(xorstr_("il2cpp_string_new"), str);
	return SpoofReturn(il2cpp::methods::new_string, str);
}

namespace variables
{
	rust::BasePlayer* local_player = nullptr;
	Vector3				local_position = Vector3();
	Matrix4x4			view_matrix = Matrix4x4();
	long				local_userid;
	int					screen_width = 1920;
	int					screen_height = 1080;
	int					image_width = 0;
	int					image_height = 0;
	Vector2				mouse_position = Vector2();
	Vector2				mouse_position_backup = Vector2();
	bool				loaded = true;
	uintptr_t			draw_label = NULL;
	uintptr_t			white_texture = NULL;
	uintptr_t			camera_list = NULL;
	uintptr_t			client_entities = NULL; 
	uintptr_t			debug_camera_address = NULL;
	std::vector<rust::entity_base> entity_draw;
	std::vector<vctr::otherEntity> helis;
	std::vector<playerEnt> players;
	player_local lcp;
	std::vector<vctr::toolCupboard> tc_list;
	std::vector<vctr::turrets> turret_list;
	std::vector<vctr::hackableCrate> hackablecrate_list;
}

std::map<KeyCode, std::string> keyCodeToString = {
		{KeyCode::Backspace, "Backspace"},
		{KeyCode::Delete, "Delete"},
		{KeyCode::Tab, "Tab"},
		{KeyCode::Clear, "Clear"},
		{KeyCode::Return, "Return"},
		{KeyCode::Pause, "Pause"},
		{KeyCode::Escape, "Escape"},
		{KeyCode::Space, "Space"},
		{KeyCode::Keypad0, "Keypad0"},
		{KeyCode::Keypad1, "Keypad1"},
		{KeyCode::Keypad2, "Keypad2"},
		{KeyCode::Keypad3, "Keypad3"},
		{KeyCode::Keypad4, "Keypad4"},
		{KeyCode::Keypad5, "Keypad5"},
		{KeyCode::Keypad6, "Keypad6"},
		{KeyCode::Keypad7, "Keypad7"},
		{KeyCode::Keypad8, "Keypad8"},
		{KeyCode::Keypad9, "Keypad9"},
		{KeyCode::KeypadPeriod, "KeypadPeriod"},
		{KeyCode::KeypadDivide, "KeypadDivide"},
		{KeyCode::KeypadMultiply, "KeypadMultiply"},
		{KeyCode::KeypadMinus, "KeypadMinus"},
		{KeyCode::KeypadPlus, "KeypadPlus"},
		{KeyCode::KeypadEnter, "KeypadEnter"},
		{KeyCode::KeypadEquals, "KeypadEquals"},
		{KeyCode::UpArrow, "UpArrow"},
		{KeyCode::DownArrow, "DownArrow"},
		{KeyCode::RightArrow, "RightArrow"},
		{KeyCode::LeftArrow, "LeftArrow"},
		{KeyCode::Insert, "Insert"},
		{KeyCode::Home, "Home"},
		{KeyCode::End, "End"},
		{KeyCode::PageUp, "PageUp"},
		{KeyCode::PageDown, "PageDown"},
		{KeyCode::F1, "F1"},
		{KeyCode::F2, "F2"},
		{KeyCode::F3, "F3"},
		{KeyCode::F4, "F4"},
		{KeyCode::F5, "F5"},
		{KeyCode::F6, "F6"},
		{KeyCode::F7, "F7"},
		{KeyCode::F8, "F8"},
		{KeyCode::F9, "F9"},
		{KeyCode::F10, "F10"},
		{KeyCode::F11, "F11"},
		{KeyCode::F12, "F12"},
		{KeyCode::F13, "F13"},
		{KeyCode::F14, "F14"},
		{KeyCode::F15, "F15"},
		{KeyCode::Alpha0, "0"},
		{KeyCode::Alpha1, "1"},
		{KeyCode::Alpha2, "2"},
		{KeyCode::Alpha3, "3"},
		{KeyCode::Alpha4, "4"},
		{KeyCode::Alpha5, "5"},
		{KeyCode::Alpha6, "6"},
		{KeyCode::Alpha7, "7"},
		{KeyCode::Alpha8, "8"},
		{KeyCode::Alpha9, "9"},
		{KeyCode::Exclaim, "Exclaim"},
		{KeyCode::DoubleQuote, "DoubleQuote"},
		{KeyCode::Hash, "Hash"},
		{KeyCode::Dollar, "Dollar"},
		{KeyCode::Percent, "Percent"},
		{KeyCode::Ampersand, "Ampersand"},
		{KeyCode::Quote, "Quote"},
		{KeyCode::LeftParen, "LeftParen"},
		{KeyCode::RightParen, "RightParen"},
		{KeyCode::Asterisk, "Asterisk"},
		{KeyCode::Plus, "Plus"},
		{KeyCode::Comma, "Comma"},
		{KeyCode::Minus, "Minus"},
		{KeyCode::Period, "Period"},
		{KeyCode::Slash, "Slash"},
		{KeyCode::Colon, "Colon"},
		{KeyCode::Semicolon, "Semicolon"},
		{KeyCode::Less, "Less"},
		{KeyCode::Equals, "Equals"},
		{KeyCode::Greater, "Greater"},
		{KeyCode::Question, "Question"},
		{KeyCode::At, "At"},
		{KeyCode::LeftBracket, "LeftBracket"},
		{KeyCode::Backslash, "Backslash"},
		{KeyCode::RightBracket, "RightBracket"},
		{KeyCode::Caret, "Caret"},
		{KeyCode::Underscore, "Underscore"},
		{KeyCode::BackQuote, "BackQuote"},
		{KeyCode::A, "A"},
		{KeyCode::B, "B"},
		{KeyCode::C, "C"},
		{KeyCode::D, "D"},
		{KeyCode::E, "E"},
		{KeyCode::F, "F"},
		{KeyCode::G, "G"},
		{KeyCode::H, "H"},
		{KeyCode::I, "I"},
		{KeyCode::J, "J"},
		{KeyCode::K, "K"},
		{KeyCode::L, "L"},
		{KeyCode::M, "M"},
		{KeyCode::N, "N"},
		{KeyCode::O, "O"},
		{KeyCode::P, "P"},
		{KeyCode::Q, "Q"},
		{KeyCode::R, "R"},
		{KeyCode::S, "S"},
		{KeyCode::T, "T"},
		{KeyCode::U, "U"},
		{KeyCode::V, "V"},
		{KeyCode::W, "W"},
		{KeyCode::X, "X"},
		{KeyCode::Y, "Y"},
		{KeyCode::Z, "Z"},
		{KeyCode::LeftCurlyBracket, "LeftCurlyBracket"},
		{KeyCode::Pipe, "Pipe"},
		{KeyCode::RightCurlyBracket, "RightCurlyBracket"},
		{KeyCode::Tilde, "Tilde"},
		{KeyCode::Numlock, "Numlock"},
		{KeyCode::CapsLock, "CapsLock"},
		{KeyCode::ScrollLock, "ScrollLock"},
		{KeyCode::RightShift, "RightShift"},
		{KeyCode::LeftShift, "LeftShift"},
		{KeyCode::RightControl, "RightControl"},
		{KeyCode::LeftControl, "LeftControl"},
		{KeyCode::RightAlt, "RightAlt"},
		{KeyCode::LeftAlt, "LeftAlt"},
		{KeyCode::LeftCommand, "LeftCommand"},
		{KeyCode::LeftApple, "LeftApple"},
		{KeyCode::LeftWindows, "LeftWindows"},
		{KeyCode::RightCommand, "RightCommand"},
		{KeyCode::RightApple, "RightApple"},
		{KeyCode::RightWindows, "RightWindows"},
		{KeyCode::AltGr, "AltGr"},
		{KeyCode::Help, "Help"},
		{KeyCode::Pr, "Pr"},
		{KeyCode::SysReq, "SysReq"},
		{KeyCode::Break, "Break"},
		{KeyCode::Menu, "Menu"},
		{KeyCode::Mouse1, "Mouse1"},
		{KeyCode::Mouse2, "Mouse2"},
		{KeyCode::Mouse3, "Mouse3"},
		{KeyCode::Mouse4, "Mouse4"},
		{KeyCode::Mouse5, "Mouse5"},
		{KeyCode::Mouse6, "Mouse6"},
		{KeyCode::Mouse7, "Mouse7"},
		{KeyCode::Unbound, "Unbound" }
};

std::map<std::string, std::string> weapon_names = {
		{"lmg.m249", "M249"},
		{"rifle.ak", "Assault Rifle"},
		{"rifle.lr300", "LR-300"},
		{"rifle.bolt", "Bolt Action"},
		{"crossbow", "Crossbow"},
		{"pistol.eoka", "Eoka"},
		{"shotgun.double", "Double shotgun"},
		{"smg.2", "Custom SMG"},
		{"hmlmg", "HMLMG"},
		{"rifle.l96", "L96"},
		{"bow.hunting", "Bow"},
		{"flamethrower", "Flame Thrower"},
		{"pistol.m92", "M92"},
		{"rifle.m39", "M39"},
		{"grenade.molotov", "Molotov"},
		{"smg.mp5", "MP5"},
		{"multiplegrenadelauncher", "MGL"},
		{"pistol.nailgun", "Nailgun"},
		{"shotgun.pump", "Pump Shotgun"},
		{"pistol.prototype17", "Prototype17"},
		{"pistol.python", "Python"},
		{"pistol.revolver", "Revolver"},
		{"rocket.launcher", "Launcher"},
		{"rifle.semiauto", "SAR"},
		{"pistol.semiauto", "Semi-Pistol"},
		{"smg.thompson", "Thompson"},
		{"shotgun.spas12", "Spas-12"},
		{"shotgun.waterpipe", "Waterpipe"},
		{"spear.wooden", "Spear"},
		{"speargun", "Speargun"},
		{"grenade.beancan", "Beancan"},
		{"grenade.f1", "Grenade"},
		{"paddle", "Paddle"},
		{"salvaged.cleaver", "SCleaver"},
		{"door.hinged.toptier", "Door"},
		{"door.double.hinged.toptier", " Door"},
		{"lock.code", "Lock"},
		{"building.planner", "BP"},
		{"wall.frame.garagedoor", "Door"},
		{"gates.external.high.stone", "Gate"},
		{"wall.external.high.stone", "Wall"},
		{"gates.external.high.wood", "Gate"},
		{"wall.external.high", "Wall"},
		{"ladder.wooden.wall", "Ladder"},
		{"door.hinged.wood", "Door"},
		{"door.double.hinged.wood", "Door"},
		{"door.double.hinged.metal", "Door"},
		{"door.hinged.metal", "Door"},
		{"cupboard.tool", "TC"},
		{"wall.window.glass.reinforced", "Window"},
		{"compound.bow", "Compound Bow"},
		{"wall.external.high.ice", "Wall"},
		{"bed", "Bed"},
		{"sleepingbag", "Bag"},
		{"syringe.medical", "Syringe"}
};

namespace functions
{
	int my_memcmp(const void* str1, const void* str2, size_t count)
	{
		const unsigned char* s1 = (const unsigned char*)str1;
		const unsigned char* s2 = (const unsigned char*)str2;

		while (count-- > 0)
		{
			if (*s1++ != *s2++)
				return s1[-1] < s2[-1] ? -1 : 1;
		}
		return 0;
	}

	int strCmp(const char* s1, const char* s2)
	{
		std::hash<std::string_view> char_hash;
		auto class_crc = char_hash(s1);
		auto object_crc = char_hash(s2);
		if (class_crc == object_crc)
			return 0;
		else
			return 1;
	}

	size_t my_strlen(const char* str)
	{
		const char* s;

		for (s = str; *s; ++s)
			;
		return (s - str);
	}

	char* memstr(char* haystack, const char* needle, int size)
	{
		char* p;
		char needlesize = my_strlen(needle);
		for (p = haystack; p <= (haystack - needlesize + size); p++) {
			if (memcmp(p, needle, needlesize) == 0)
			{
				//std::cout << "[" << __FUNCTION__ << "] Returning: " << p << std::endl;
				return p;
			}
		}
		return NULL;
	}

	std::string ws2s(const std::wstring& wstr)
	{
		std::string str(wstr.begin(), wstr.end());
		return str;
	}

	std::wstring s2ws(const std::string& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		return wstrTo;
	}

	std::wstring read_unicode(uint64_t address, std::size_t size)
	{
		const auto buffer = std::make_unique<wchar_t[]>(size);
		memcpy(buffer.get(), reinterpret_cast<const void*>(address), size * 2); //Try Rusts memcpy. public static class UnsafeUtility::MemCpy(void* destination, void* source, long size)
		return std::wstring(buffer.get());
	}

	std::string read_ascii(uint64_t address, std::size_t size)
	{
		return std::string(reinterpret_cast<const char*>(address));
	}

	const char* read_ascii_alt(uint64_t address)
	{
		return reinterpret_cast<const char*>(address);
	}

	std::string format_time(float seconds_float) {
		// Convert float to integer (rounding down)
		int seconds = static_cast<int>(std::floor(seconds_float));

		int hours = seconds / 3600;
		seconds %= 3600;
		int minutes = seconds / 60;
		seconds %= 60;

		std::string result = "";

		if (hours > 0) {
			result += std::to_string(hours) + "h ";
		}
		if (hours > 0 || minutes > 0) {
			result += std::to_string(minutes) + "m ";
		}
		result += std::to_string(seconds) + "s";

		return result;
	}

	bool getPositionFromView(Vector3& out, Matrix4x4 m)
	{
		//if (!m) return false;
		float	A = m._22 * m._33 - m._32 * m._23,
			B = m._32 * m._13 - m._12 * m._33,
			C = m._12 * m._23 - m._22 * m._13,
			Z = m._11 * A + m._21 * B + m._31 * C;

		if (abs(Z) < 0.0001) return false;

		float	D = m._31 * m._23 - m._21 * m._33,
			E = m._11 * m._33 - m._31 * m._13,
			F = m._21 * m._13 - m._11 * m._23,
			G = m._21 * m._32 - m._31 * m._22,
			H = m._31 * m._12 - m._11 * m._32,
			K = m._11 * m._22 - m._21 * m._12;

		out.x = -(A * m._41 + D * m._42 + G * m._43) / Z;
		out.y = -(B * m._41 + E * m._42 + H * m._43) / Z;
		out.z = -(C * m._41 + F * m._42 + K * m._43) / Z;
		return true;
	}

	bool WorldToScreen(const Vector3& EntityPos, Vector2& ScreenPos)
	{
		if (true)
		{
			Vector3 TransVec = Vector3(variables::view_matrix._14, variables::view_matrix._24, variables::view_matrix._34);
			Vector3 RightVec = Vector3(variables::view_matrix._11, variables::view_matrix._21, variables::view_matrix._31);
			Vector3 UpVec = Vector3(variables::view_matrix._12, variables::view_matrix._22, variables::view_matrix._32);
			float w = Math::Dot(TransVec, EntityPos) + variables::view_matrix._44;
			if (w < 0.098f) return false;
			float y = Math::Dot(UpVec, EntityPos) + variables::view_matrix._42;
			float x = Math::Dot(RightVec, EntityPos) + variables::view_matrix._41;
			ScreenPos = Vector2((variables::screen_width / 2) * (1.f + x / w), (variables::screen_height / 2) * (1.f - y / w));
			if (ScreenPos.x > (0 - 130) && (variables::screen_width + 130) > ScreenPos.x) {
				if (ScreenPos.y > (0 - 130) && (variables::screen_height + 130) > ScreenPos.y) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	laddy_list<uintptr_t>* get2renderers(uintptr_t multimesh)
	{
		uintptr_t method = il2cpp::method(xorstr_("SkinnedMultiMesh"), xorstr_("get_Renderers"), 0);
		if (!(method))
		{
			return NULL;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return NULL;
		}

		auto get_renderers = reinterpret_cast<laddy_list<uintptr_t>*(*)(uintptr_t)>(method_ptr);
		return SpoofReturn(get_renderers, multimesh);

	}

	uintptr_t get_material(uintptr_t renderer)
	{
		uintptr_t method = offsets::get_material; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return NULL;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return NULL;
		}

		auto get_material = reinterpret_cast<uintptr_t(*)(uintptr_t)>(method_ptr);
		return SpoofReturn(get_material, renderer);
	}

	uintptr_t get_shader(uintptr_t material)
	{
		uintptr_t method = offsets::get_shader; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return NULL;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return NULL;
		}

		auto get_shader = reinterpret_cast<uintptr_t(*)(uintptr_t)>(method_ptr);
		return SpoofReturn(get_shader, material);
	}

	uintptr_t find(const char* shader)
	{
		uintptr_t method = offsets::find; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return NULL;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return NULL;
		}

		auto get_shader = reinterpret_cast<uintptr_t(*)(il2cpp::il2cppstring * shader)>(method_ptr);
		return SpoofReturn(get_shader, il2cpp::il2cppstring::New(shader));
	}

	void set_shader(uintptr_t material, uintptr_t shader)
	{
		uintptr_t method = offsets::set_shader; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto set_color = reinterpret_cast<void(*)(uintptr_t material, uintptr_t shader)>(method_ptr);
		SpoofReturn(set_color, material, shader);
	}

	void set_color(uintptr_t material, const char* name, Vector4 color)
	{
		uintptr_t method = offsets::set_color; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto set_color = reinterpret_cast<void(*)(uintptr_t material, il2cpp::il2cppstring * name, Vector4 color)>(method_ptr);

		SpoofReturn(set_color, material, il2cpp::il2cppstring::New(name), color);
	}

	uintptr_t scan_for_camera_list()
	{
		const auto base = LI_MODULE(("UnityPlayer.dll")).get<uintptr_t>(); //reinterpret_cast<uint64_t>(GetModuleHandle(xorstr_("UnityPlayer.dll")));

		if (!base)
			return NULL;

		const auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
		const auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS64*>(base + dos_header->e_lfanew);

		uint64_t data_base;
		uint64_t data_size;

		for (int i = 0;;)
		{
			const auto section = reinterpret_cast<IMAGE_SECTION_HEADER*>(
				base + dos_header->e_lfanew + // nt_header base
				sizeof(IMAGE_NT_HEADERS64) +  // start of section headers
				(i * sizeof(IMAGE_SECTION_HEADER))); // section header at our index

			//std::cout << "[" << __FUNCTION__ << "] Section Name: " << (char*)section->Name << std::endl;

			if (strCmp((char*)section->Name, xorstr_(".data")) == 0)
			{
				data_base = section->VirtualAddress + base;
				data_size = section->SizeOfRawData;
				break;
			}

			i++;

			if (i >= nt_header->FileHeader.NumberOfSections)
			{
				//std::cout << "[" << __FUNCTION__ << "] Section not found" << std::endl;
				//DEBUG("[!] Section not found\n"));
				return NULL;
			}
		}

		uint64_t camera_table = 0;

		const auto camera_string = memstr((char*)data_base, xorstr_("AllCameras"), data_size);

		for (auto walker = (uint64_t*)camera_string; walker > (uint64_t*)0; walker -= 1)
		{
			if (*walker > 0x100000 && *walker < 0xF00000000000000)
			{
				// [[[[unityplayer.dll + ctable offset]]] + 0x30] = Camera
				//std::cout << "[" << __FUNCTION__ << "] camera_table found" << std::endl;
				camera_table = *walker;
				break;
			}
		}

		if (camera_table)
		{
			//std::cout << "[" << __FUNCTION__ << "] Returning Camera Table: " << camera_table << std::endl;
			return camera_table;
		}

		//DEBUG("[!] Unable to locate camera\n"));
		return NULL;
	}

	bool update_view_matrix()
	{
		uintptr_t maincam = getMainCamera(); //variables::camera_list; //scan_for_camera_list();
		
		if (maincam) {
			//uintptr_t camera_table = *reinterpret_cast<uintptr_t*>(camera_list);
			//if (!camera_table)
			//{
			//	//std::cout << "[" << __FUNCTION__ << "] !object_list" << std::endl;
			//	return false;
			//}

			uintptr_t camera = *reinterpret_cast<uintptr_t*>(maincam + 0x10);
			if (!camera)
			{
				//std::cout << "[" << __FUNCTION__ << "] !object_list" << std::endl;
				return false;
			}
			variables::view_matrix = *reinterpret_cast<Matrix4x4*>(camera + 0x2E4);
			return true;
		}
		else {
			return false;
		}
	}

	Vector3 get_camera_pos()
	{
		auto camera_list = variables::camera_list; //scan_for_camera_list();
		if (!(camera_list))
		{
			return Vector3();
		}

		uintptr_t camera_table = *reinterpret_cast<uintptr_t*>(camera_list);
		if (!(camera_table))
		{
			return Vector3();
		}

		uintptr_t camera = *reinterpret_cast<uintptr_t*>(camera_table);
		if (!(camera))
		{
			return Vector3();
		}

		return *reinterpret_cast<Vector3*>(camera + 0x42C);
	}
	uintptr_t get_texture(uintptr_t sprite)
	{
		uintptr_t method = offsets::get_texture; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return NULL;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return NULL;
		}

		auto get_texture = reinterpret_cast<uintptr_t(*)(uintptr_t)>(method_ptr);
		if (!get_texture)
			return NULL;
		return SpoofReturn(get_texture, sprite);
	}

	Vector3 get_mouse_pos()
	{
		uintptr_t method = offsets::get_mousePosition;
		if (!(method))
		{
			return Vector3();
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return Vector3();
		}

		auto get_mousePosition = reinterpret_cast<Vector3(*)()>(method_ptr);
		return get_mousePosition();
	}

	int get_screen_width()
	{
		uintptr_t method = offsets::screen_get_width;
		if (!(method))
		{
			return int();
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return int();
		}

		auto get_screen_position = reinterpret_cast<int(*)()>(method_ptr);
		return SpoofReturn(get_screen_position);
	}

	int get_screen_height()
	{
		uintptr_t method = offsets::screen_get_height;
		if (!(method))
		{
			return int();
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return int();
		}

		auto get_screen_position = reinterpret_cast<int(*)()>(method_ptr);
		return SpoofReturn(get_screen_position);
	}

	void ignore_layer_collision(layer the_layer, layer ignore_layer, bool should_collide)
	{
		uintptr_t method = offsets::ignore_layer_collision;
		if (!(method))
		{
			return;
		}

		auto ignore_layer_func = reinterpret_cast<void(*)(layer, layer, bool)>(method);
		SpoofReturn(ignore_layer_func, the_layer, ignore_layer, should_collide);
	}

	bool get_key_down(KeyCode key)
	{
		uintptr_t method = offsets::input_get_key_down;
		if (!(method))
		{
			return false;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return false;
		}

		auto get_keydown = reinterpret_cast<bool(*)(KeyCode)>(method_ptr);
		return SpoofReturn(get_keydown, key);
	}

	bool get_key_up(KeyCode key)
	{
		uintptr_t method = offsets::input_get_key_up; //il2cpp::method(xorstr_("Input"), xorstr_("GetKeyUp"), 1, xorstr_("UnityEngine"));
		if (!(method))
		{
			return false;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return false;
		}

		auto get_keydown = reinterpret_cast<bool(*)(KeyCode)>(method_ptr);
		return SpoofReturn(get_keydown, key);
	}

	bool get_key(KeyCode key)
	{
		if (key != KeyCode::Unbound) {
			uintptr_t method = offsets::input_get_key; //il2cpp::method(xorstr_("Input"), xorstr_("GetKey"), 1, xorstr_("UnityEngine"));
			if (!(method))
			{
				return false;
			}

			uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
			if (!(method_ptr))
			{
				return false;
			}

			auto get_keydown = reinterpret_cast<bool(*)(KeyCode)>(method_ptr);
			return SpoofReturn(get_keydown, key);
		}
		else {
			return false;
		}
	}

	bool get_mouse_button(int* button)
	{
		uintptr_t method = offsets::input_get_mouse_button; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return false;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return false;
		}

		auto get_mouse_button = reinterpret_cast<bool(*)(int(*))>(method_ptr);

		//auto get_current = reinterpret_cast<uintptr_t(*)()>(method_ptr);
		return SpoofReturn(get_mouse_button, button);
	}


	bool line_of_sight(Vector3 start, Vector3 end, Layers layer_mask, float padding)
	{
		uintptr_t method = offsets::gamephysics_line_of_site; //il2cpp::method(xorstr_("GamePhysics"), xorstr_("LineOfSight"), 4);
		if (!(method))
		{
			return false;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return false;
		}

		auto get_line_of_sight = reinterpret_cast<bool(*)(Vector3, Vector3, Layers, float)>(method_ptr);
		return SpoofReturn(get_line_of_sight, start, end, layer_mask, padding);
	}

	bool is_visible(Vector3 start, Vector3 end)
	{
		auto layer = (int)Layers::ProjectileLineOfSightCheck | (int)Layers::Terrain;

		return line_of_sight(start, end, Layers(layer), 0);
	}

	float clamp(float value, float min, float max)
	{
		uintptr_t method = offsets::mathf_clamp; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return false;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return false;
		}

		auto clamp = reinterpret_cast<float(*)(float, float, float)>(method_ptr);
		return SpoofReturn(clamp, value, min, max);
	}

	float mathf_atan2(float y, float x)
	{
		uintptr_t method = offsets::mathf_atan2; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
		
			return 0.f;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			
			return 0.f;
		}

		auto atan2 = reinterpret_cast<float(*)(float, float)>(method_ptr);
		return SpoofReturn(atan2, y, x);
	}

	float mathf_max(float a, float b)
	{
		uintptr_t method = offsets::mathf_max; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return 0.f;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return 0.f;
		}

		auto atan2 = reinterpret_cast<float(*)(float, float)>(method_ptr);
		return SpoofReturn(atan2, a, b);
	}

	float mathf_abs(float f)
	{
		uintptr_t method = offsets::mathf_abs; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return 0.f;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return 0.f;
		}

		auto atan2 = reinterpret_cast<float(*)(float)>(method_ptr);
		return SpoofReturn(atan2, f);
	}

	uintptr_t get_current()
	{
		uintptr_t method = offsets::event_get_current; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return NULL;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return NULL;
		}

		auto get_current = reinterpret_cast<uintptr_t(*)()>(method_ptr);
		return SpoofReturn(get_current);
	}

	EventType get_event_type(uintptr_t current)
	{
		uintptr_t method = offsets::event_get_type; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return EventType::Ignore;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return EventType::Ignore;
		}

		auto get_type = reinterpret_cast<EventType(*)(uintptr_t)>(method_ptr);
		return SpoofReturn(get_type, current);
	}

	KeyCode get_keyCode(uintptr_t current)
	{
		uintptr_t method = offsets::event_get_keycode; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return KeyCode::Semicolon;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return KeyCode::Semicolon;
		}

		auto get_keyCode = reinterpret_cast<KeyCode(*)(uintptr_t)>(method_ptr);

		//auto get_current = reinterpret_cast<uintptr_t(*)()>(method_ptr);
		return SpoofReturn(get_keyCode, current);
	}


	uintptr_t get_draw_skin()
	{
		uintptr_t method = offsets::gui_get_skin; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return false;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return false;
		}

		auto get_skin = reinterpret_cast<uintptr_t(*)()>(method_ptr);
		return SpoofReturn(get_skin);
	}

	uintptr_t get_white_texture()
	{
		uintptr_t method = offsets::texture2d_get_whitetexture; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return false;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return false;
		}

		auto get_whiteTexture = reinterpret_cast<uintptr_t(*)()>(method_ptr);
		return SpoofReturn(get_whiteTexture);
	}

	void set_draw_font_size(uintptr_t label, int size)
	{
		uintptr_t method = offsets::guistyle_set_fontsize; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto set_fontSize = reinterpret_cast<void(*)(uintptr_t label, int size)>(method_ptr);
		SpoofReturn(set_fontSize, label, size);
	}

	void set_draw_color(Vector4 color)
	{
		uintptr_t method = offsets::gui_set_color; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto set_color = reinterpret_cast<void(*)(Vector4 color)>(method_ptr);
		SpoofReturn(set_color, color);
	}

	void set_draw_alignment(uintptr_t label, uintptr_t value)
	{
		uintptr_t method = offsets::guistyle_set_alignment; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto set_alignment = reinterpret_cast<void(*)(uintptr_t label, uintptr_t value)>(method_ptr);
		SpoofReturn(set_alignment, label, value);
	}

	void draw_texture(Vector4 position, uintptr_t texture)
	{
		uintptr_t method = offsets::gui_drawtexture; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto DrawTexture = reinterpret_cast<void(*)(Vector4 position, uintptr_t texture)>(method_ptr);
		SpoofReturn(DrawTexture, position, texture);
	}
	void draw_texture2(Vector2 position, uintptr_t texture)
	{
		uintptr_t method = offsets::gui_drawtexture; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto DrawTexture = reinterpret_cast<void(*)(Vector2 position, uintptr_t texture)>(method_ptr);
		SpoofReturn(DrawTexture, position, texture);
	}

	void outline_box(Vector2 position, Vector2 size, Vector4 color, float girth)
	{
		if (!variables::white_texture)
			variables::white_texture = get_white_texture();

		set_draw_color(color);

		draw_texture(Vector4(position.x, position.y, girth, size.y), variables::white_texture);
		draw_texture(Vector4(position.x + size.x, position.y, girth, size.y + 1), variables::white_texture);
		draw_texture(Vector4(position.x, position.y, size.x, girth), variables::white_texture);
		draw_texture(Vector4(position.x, position.y + size.y, size.x, girth), variables::white_texture);
	}

	void fill_box(Vector4 position, Vector4 color)
	{
		if (!variables::white_texture)
			variables::white_texture = get_white_texture();
		set_draw_color(color);
		draw_texture(position, variables::white_texture);
	}
	void fill_box2(Vector2 position, Vector4 color)
	{
		if (!variables::white_texture)
			variables::white_texture = get_white_texture();
		set_draw_color(color);
		draw_texture2(position, variables::white_texture);
	}
	void draw_pixel(int x, int y)
	{
		Vector4 position = { static_cast<float>(x), static_cast<float>(y), 1.0f, 1.0f };
		draw_texture(position, variables::white_texture);  // Assuming variables::white_texture is a white 1x1 texture
	}
	void line(Vector2 point1, Vector2 point2, Vector4 color)
	{
		set_draw_color(color);

		int x1 = point1.x, y1 = point1.y, x2 = point2.x, y2 = point2.y;
		bool steep = abs(y2 - y1) > abs(x2 - x1);

		if (steep)
		{
			std::swap(x1, y1);
			std::swap(x2, y2);
		}

		if (x1 > x2)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		int dx = x2 - x1;
		int dy = abs(y2 - y1);
		int error = dx / 2;
		int ystep = (y1 < y2) ? 1 : -1;
		int y = y1;

		for (int x = x1; x <= x2; x++)
		{
			if (steep)
			{
				draw_pixel(y, x);
			}
			else
			{
				draw_pixel(x, y);
			}

			error -= dy;
			if (error < 0)
			{
				y += ystep;
				error += dx;
			}
		}
	}

	void draw_triangle(Vector2 point1, Vector2 point2, Vector2 point3, Vector4 color)
	{
		line(point1, point2, color);
		line(point2, point3, color);
		line(point3, point1, color);
	}

	void draw_outline_circle(int centerX, int centerY, int radius, Vector4 color)
	{
		set_draw_color(color);

		int x = radius;
		int y = 0;
		int radiusError = 1 - x;

		while (x >= y)
		{
			// Draw the circle points, but only the ones on the border
			draw_pixel(centerX + x, centerY + y);
			draw_pixel(centerX - x, centerY + y);
			draw_pixel(centerX + x, centerY - y);
			draw_pixel(centerX - x, centerY - y);
			draw_pixel(centerX + y, centerY + x);
			draw_pixel(centerX - y, centerY + x);
			draw_pixel(centerX + y, centerY - x);
			draw_pixel(centerX - y, centerY - x);

			y++;

			if (radiusError < 0)
			{
				radiusError += 2 * y + 1;
			}
			else
			{
				x--;
				radiusError += 2 * (y - x + 1);
			}
		}
	}

	void label(Vector4 position, std::string text, Vector4 color, bool centered, float font_size)
	{
		set_draw_color(color);

		set_draw_font_size(variables::draw_label, font_size);

		if (centered)
			set_draw_alignment(variables::draw_label, 0x4);
		else
			set_draw_alignment(variables::draw_label, 0x0);

		uintptr_t method = offsets::gui_label;//LI_MODULE_SAFE_("GameAssembly.dll") + 0x1A61F60; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto Label = reinterpret_cast<void(*)(Vector4 position, il2cpp::il2cppstring*)>(method_ptr);
		SpoofReturn(Label, position, il2cpp::il2cppstring::New(text.c_str()));
	}

	void outline(Vector4 position, std::string text, Vector4 color, bool centered, float font_size)
	{
		label({ position.x - 1, position.y - 1, position.z, position.w }, text, Vector4(0, 0, 0, color.w), centered, font_size);
		label({ position.x + 1, position.y - 1, position.z, position.w }, text, Vector4(0, 0, 0, color.w), centered, font_size);
		label({ position.x - 1, position.y + 1, position.z, position.w }, text, Vector4(0, 0, 0, color.w), centered, font_size);
		label({ position.x + 1, position.y + 1, position.z, position.w }, text, Vector4(0, 0, 0, color.w), centered, font_size);
		label({ position.x, position.y, position.z, position.w }, text, color, centered, font_size);
	}

	void ddraw_get()
	{
		uintptr_t method = offsets::ddraw_get; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto Get = reinterpret_cast<void(*)()>(method_ptr);
		SpoofReturn(Get);
	}

	void create(uintptr_t self, const char* shader)
	{
		uintptr_t method = offsets::create;
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto create = reinterpret_cast<void(*)(uintptr_t self, il2cpp::il2cppstring * shader)>(method_ptr);
		SpoofReturn(create, self, il2cpp::il2cppstring::New(shader));
	}

	void add_component(uintptr_t self, uintptr_t componentType)
	{
		uintptr_t method = offsets::add_component;
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto add_component = reinterpret_cast<void(*)(uintptr_t self, uintptr_t componentType)>(method_ptr);
		SpoofReturn(add_component, self, componentType);
	}



	void dont_destroy_on_load(uintptr_t target)
	{
		uintptr_t method = offsets::dont_destroy_on_load;
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto dont_destroy_on_load = reinterpret_cast<void(*)(uintptr_t target)>(method_ptr);
		SpoofReturn(dont_destroy_on_load, target);
	}

	uintptr_t CreatePrefab(const char* prefab, bool active)
	{
		uintptr_t method = offsets::gamemanager_createprefab; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return NULL;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return NULL;
		}

		auto create_prefab = reinterpret_cast<uintptr_t(*)(il2cpp::il2cppstring*, bool)>(method_ptr);
		return SpoofReturn(create_prefab, il2cpp::il2cppstring::New(prefab), active);
	}

	void Destroy(uintptr_t object)
	{
		uintptr_t method = offsets::gamemanager_destroy; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return;
		}

		auto create_prefab = reinterpret_cast<void(*)(uintptr_t)>(method_ptr);
		return SpoofReturn(create_prefab, object);
	}

	float get_time()
	{
		uintptr_t method = offsets::time_get_time; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return 0.f;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return 0.f;
		}

		auto get_time = reinterpret_cast<float(*)()>(method_ptr);
		return SpoofReturn(get_time);
	}

	float get_deltaTime()
	{
		uintptr_t method = offsets::time_get_deltaTime; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			//std::cout << "dT method false!" << std::endl;
			return 0.f;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			//std::cout << "dT method_ptr false!" << std::endl;
			return 0.f;
		}

		auto get_deltaTime = reinterpret_cast<float(*)()>(method_ptr);
		return SpoofReturn(get_deltaTime);
	}

	float random_range(float min, float max)
	{
		uintptr_t method = offsets::randomrange; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return min + (max - min);
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return min + (max - min);
		}

		auto range = reinterpret_cast<float(*)(float min, float max)>(method_ptr);
		return SpoofReturn(range, min, max);
	}

	bool physics_checkcapsule(Vector3 start, Vector3 end, float radius, int layer_mask, QueryTriggerInteraction querytriggerinteraction)
	{
		uintptr_t method = offsets::physics_checkcapsule; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			//std::cout << "PCC method false!" << std::endl;
			return false;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			//std::cout << "PCC method_ptr false!" << std::endl;
			return false;
		}

		auto checkcapsule = reinterpret_cast<bool(*)(Vector3 start, Vector3 end, float radius, int layer_mask, QueryTriggerInteraction querytriggerinteraction)>(method_ptr);
		return SpoofReturn(checkcapsule, start, end, radius, layer_mask, querytriggerinteraction);
	}
}

std::wstring get_name(uintptr_t object)
{
	if (!(object))
	{
		return std::wstring();
	}

	uintptr_t address = offsets::object_name;
	if (!address)
		return std::wstring();

	il2cpp::il2cppstring* entity_name = *reinterpret_cast<il2cpp::il2cppstring**>(object + address);
	if (!(reinterpret_cast<uintptr_t>(entity_name)))
		return std::wstring();

	return entity_name->buffer;
}

