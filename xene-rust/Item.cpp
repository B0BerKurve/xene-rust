#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	BaseEntity* Item::get_held_entity()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!entity)
			return nullptr;

		auto held = *reinterpret_cast<BaseEntity**>(entity + offsets::item_heldEntity);
		if (!held)
			return nullptr;

		return held;
	}

	uintptr_t Item::get_sprite()
	{
		uintptr_t method = offsets::get_iconSprite; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!(method))
		{
			return NULL;
		}

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!(method_ptr))
		{
			return NULL;
		}

		auto get_sprite = reinterpret_cast<uintptr_t(*)(uintptr_t)>(method_ptr);
		if (!get_sprite)
			return NULL;
		return SpoofReturn(get_sprite, reinterpret_cast<uintptr_t>(this));
	}
	void Item::fakefire()
	{

		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		auto held = this->get_held_entity();
		if (!held)
			return;

		BaseProjectile* base_projectile = reinterpret_cast<BaseProjectile*>(held);
		if (!base_projectile)
			return;

		if (!(entity))
			return;
		base_projectile->send_signal_broadcast(0, (""));
	}
	void Item::recoilControl(float value)
	{
		{
			rust::Item* held_item = variables::local_player->get_held_item();
			if (held_item && (reinterpret_cast<uintptr_t>(held_item)))
			{
				if (held_item->is_gun())
				{
					auto heldEntity = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(held_item) + offsets::item_heldEntity);
					if ((heldEntity))
					{
						Vector2 pitch_bounds;
						Vector2 yaw_bounds;
						const auto mod_raw = value;
						auto recoil = *reinterpret_cast<uintptr_t*>(heldEntity + offsets::baseprojectile_recoil); //public RecoilProperties recoil; // 0x2D8
						if ((recoil))
						{
							recoil = *reinterpret_cast<uintptr_t*>(recoil + 0x78);
							if ((recoil))
							{
								if (mod_raw > 0)
								{
									if (held_item->get_recoil_min_max(pitch_bounds, yaw_bounds))
									{
										const auto mod = functions::clamp(mod_raw, 0.f, 1.f);

										memory::write<float>(recoil + 0x18, yaw_bounds.x * mod);
										memory::write<float>(recoil + 0x1C, yaw_bounds.y * mod);
										memory::write<float>(recoil + 0x20, pitch_bounds.x * mod);
										memory::write<float>(recoil + 0x24, pitch_bounds.y * mod);
									}
								}
								else
								{
									memory::write<float>(recoil + 0x18, 0.f);
									memory::write<float>(recoil + 0x1C, 0.f);
									memory::write<float>(recoil + 0x20, 0.f);
									memory::write<float>(recoil + 0x24, 0.f);
								}
							}
							else
							{
								recoil = *reinterpret_cast<uintptr_t*>(heldEntity + offsets::baseprojectile_recoil);
								if (recoil)
								{
									if (mod_raw > 0)
									{
										if (held_item->get_recoil_min_max(pitch_bounds, yaw_bounds))
										{
											const auto mod = functions::clamp(mod_raw / 100.f, 0.f, 1.f);

											memory::write<float>(recoil + 0x18, yaw_bounds.x * mod);
											memory::write<float>(recoil + 0x1C, yaw_bounds.y * mod);
											memory::write<float>(recoil + 0x20, pitch_bounds.x * mod);
											memory::write<float>(recoil + 0x24, pitch_bounds.y * mod);
										}
									}
									else
									{
										memory::write<float>(recoil + 0x18, 0.f);
										memory::write<float>(recoil + 0x1C, 0.f);
										memory::write<float>(recoil + 0x20, 0.f);
										memory::write<float>(recoil + 0x24, 0.f);
									}
								}
							}
						}
					}
				}
			}
		}
	}


	int Item::get_amount()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return int();

		uintptr_t address = offsets::item_amount; //il2cpp::value(xorstr_("Item"), xorstr_("amount"));
		if (!address)
			return int();

		return *reinterpret_cast<int*>(entity + address);
	}

	int32_t Item::get_definition_id()
	{
		const auto     item_definition = *reinterpret_cast<uintptr_t*>((uintptr_t)this + offsets::item_info);
		if (!item_definition)
			return 0;

		return *reinterpret_cast<int32_t*>(item_definition + 0x18);
	}

	int Item::get_uid()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return int();

		uintptr_t address = offsets::item_uid; //il2cpp::value(xorstr_("Item"), xorstr_("uid"));
		if (!address)
			return int();

		return *reinterpret_cast<int*>(entity + address);
	}

	std::wstring Item::get_name()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return std::wstring();
		}

		uintptr_t address = offsets::item_info; //il2cpp::value(xorstr_("Item"), xorstr_("info"))
		if (!address)
			return std::wstring();

		uintptr_t info = *reinterpret_cast<uintptr_t*>(entity + address);
		if (!(info))
			return std::wstring();

		uintptr_t address2 = offsets::itemdefinition_shortname; //il2cpp::value(xorstr_("ItemDefinition"), xorstr_("shortname"));
		if (!address2)
			return std::wstring();

		il2cpp::il2cppstring* entity_name = *reinterpret_cast<il2cpp::il2cppstring**>(info + address2);
		if (!(reinterpret_cast<uintptr_t>(entity_name)))
			return std::wstring();

		return entity_name->buffer;
	}

	Vector3 Item::get_visual_position(uintptr_t object_class)
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

	bool Item::get_recoil_min_max(Vector2& pitch_bounds, Vector2& yaw_bounds)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return false;
		}

		auto item_definition = *reinterpret_cast<uintptr_t*>(entity + offsets::item_info); //public ItemDefinition info; // 0x20
		if (!(item_definition))
		{
			return false;
		}

		if (!is_gun())
		{
			return false;
		}

		unsigned int weapon_id = *reinterpret_cast<unsigned int*>(item_definition + 0x18);
		switch (weapon_id)
		{
		case 0x5c22b98e/*Assault Rifle*/: { //Updated.
			pitch_bounds.x = -2.5;
			pitch_bounds.y = -3.5;
			yaw_bounds.x = 1.5;
			yaw_bounds.y = 2.5;
			return true;
		}
		case 0xb065ec45/*Assault Rifle (ICE)*/: { //Updated.
			pitch_bounds.x = -2.5;
			pitch_bounds.y = -3.5;
			yaw_bounds.x = 1.5;
			yaw_bounds.y = 2.5;
			return true;
		}
		case 0x5eab82c3/*Bolt Action Rifle*/: { //Updated.
			pitch_bounds.x = -2;
			pitch_bounds.y = -3;
			yaw_bounds.x = -4;
			yaw_bounds.y = 4;
			return true;
		}
		case 0x34b73d71/*Compound Bow*/: { //Updated.
			pitch_bounds.x = -3;
			pitch_bounds.y = -6;
			yaw_bounds.x = -3;
			yaw_bounds.y = 3;
			return true;
		}
		case 0x7523110a/*Crossbow*/: { //Updated.
			pitch_bounds.x = -3;
			pitch_bounds.y = -6;
			yaw_bounds.x = -3;
			yaw_bounds.y = 3;
			return true;
		}
		case 0x6b1731e1/*Custom SMG*/: { //Updated.
			pitch_bounds.x = -1.5;
			pitch_bounds.y = -2;
			yaw_bounds.x = -1;
			yaw_bounds.y = 1;
			return true;
		}
		case 0xd264397f/*Double Barrel Shotgun*/: { //Updated.
			pitch_bounds.x = -10;
			pitch_bounds.y = -15;
			yaw_bounds.x = 8;
			yaw_bounds.y = 15;
			return true;
		}
		case 0x560b474f/*Hunting Bow*/: { //Updated.
			pitch_bounds.x = -3;
			pitch_bounds.y = -6;
			yaw_bounds.x = -3;
			yaw_bounds.y = 3;
			return true;
		}
		case 0xd19b0ec1/*L96 Rifle*/: { //Updated.
			pitch_bounds.x = -1;
			pitch_bounds.y = -1.5;
			yaw_bounds.x = -2;
			yaw_bounds.y = 2;
			return true;
		}
		case 0x93f69a57/*LR-300 Assault Rifle*/: { //Updated.
			pitch_bounds.x = -2;
			pitch_bounds.y = -3;
			yaw_bounds.x = -1;
			yaw_bounds.y = 1;
			return true;
		}
		case 0x84a4bb78/*M249*/: { //Updated.
			pitch_bounds.x = -3;
			pitch_bounds.y = -3;
			yaw_bounds.x = -1;
			yaw_bounds.y = 1;
			return true;
		}
		case 0x1ae5371/*M39 Rifle*/: { //Updated.
			pitch_bounds.x = -5;
			pitch_bounds.y = -7;
			yaw_bounds.x = -1.5;
			yaw_bounds.y = 1.5;
			return true;
		}
		case 0xcd2eebb5/*M92 Pistol*/: {
			pitch_bounds.x = -7;
			pitch_bounds.y = -8;
			yaw_bounds.x = -1;
			yaw_bounds.y = 1;
			return true;
		}
		case 0x4e979c37/*MP5A4*/: { //Updated
			pitch_bounds.x = -1;
			pitch_bounds.y = -3;
			yaw_bounds.x = -1;
			yaw_bounds.y = 1;
			return true;
		}
		case 0xbd092660/*Multiple Grenade Launcher*/: {
			pitch_bounds.x = -15.f;
			pitch_bounds.y = -20.f;
			yaw_bounds.x = 5.f;
			yaw_bounds.y = 10.f;
			return true;
		}
		case 0x74763261/*Nailgun*/: {
			pitch_bounds.x = -3;
			pitch_bounds.y = -6;
			yaw_bounds.x = -1;
			yaw_bounds.y = 1;
			return true;
		}
		case 0x2f686650/*Pump Shotgun*/: {
			pitch_bounds.x = -10.f;
			pitch_bounds.y = -14.f;
			yaw_bounds.x = 4.f;
			yaw_bounds.y = 8.f;
			return true;
		}
		case 0x51e52593/*Python Revolver*/: {
			pitch_bounds.x = -15.f;
			pitch_bounds.y = -16.f;
			yaw_bounds.x = -2.f;
			yaw_bounds.y = 2.f;
			return true;
		}
		case 0x26bce126/*Revolver*/: {
			pitch_bounds.x = -3.f;
			pitch_bounds.y = -6.f;
			yaw_bounds.x = -1.f;
			yaw_bounds.y = 1.f;
			return true;
		}
		case 0x1a65e87c/*Rocket Launcher*/: {
			pitch_bounds.x = -15.f;
			pitch_bounds.y = -20.f;
			yaw_bounds.x = 5.f;
			yaw_bounds.y = 10.f;
			return true;
		}
		case 0x30cf142c/*Semi-Automatic Pistol*/: {
			pitch_bounds.x = -6.f;
			pitch_bounds.y = -8.f;
			yaw_bounds.x = -2.f;
			yaw_bounds.y = 2.f;
			return true;
		}
		case 0xca10e257/*Semi-Automatic Rifle*/: {
			pitch_bounds.x = -5.f;
			pitch_bounds.y = -6.f;
			yaw_bounds.x = -1.f;
			yaw_bounds.y = 1.f;
			return true;
		}
		case 0xfd87ab32/*Spas-12 Shotgun*/: {
			pitch_bounds.x = -10.f;
			pitch_bounds.y = -14.f;
			yaw_bounds.x = 4.f;
			yaw_bounds.y = 8.f;
			return true;
		}
		case 0x97315c8b/*Thompson*/: { //Updated
			pitch_bounds.x = -1.5;
			pitch_bounds.y = -2;
			yaw_bounds.x = -1;
			yaw_bounds.y = 1;
			return true;
		}
		case 0xae80eeeb/*Waterpipe Shotgun*/: {
			pitch_bounds.x = -10.f;
			pitch_bounds.y = -14.f;
			yaw_bounds.x = 4.f;
			yaw_bounds.y = 8.f;
			return true;
		}
		case 0xb79b8d5f/*HMLMG*/: { //Updated.
			pitch_bounds.x = -3;
			pitch_bounds.y = -4;
			yaw_bounds.x = -1;
			yaw_bounds.y = -1.5;
			return true;
		}
		default: {
			return false;
		}
		}
	}

	void Item::set_always_eoka(float value)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return;
		}

		auto item_name = get_name();
		if (item_name.find(xorstr_(L"eoka")) != std::string::npos)
		{
			auto held = *reinterpret_cast<uintptr_t*>(entity + offsets::item_heldEntity);
			if (!(held))
			{
				return;
			}

			memory::write<float>(held + offsets::flintstrikeweapon_successFraction, value);
		}
	}

	void Item::set_aim_sway(float value)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return;
		}

		auto held = *reinterpret_cast<uintptr_t*>(entity + offsets::item_heldEntity);
		if (!(held))
		{
			return;
		}

		if (!is_gun())
		{
			return;
		}

		memory::write<float>(held + offsets::baseprojectile_aimSway, value);
	}

	void Item::set_aim_sway_speed(float value)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return;
		}

		auto held = *reinterpret_cast<uintptr_t*>(entity + offsets::item_heldEntity);
		if (!(held))
		{
			return;
		}

		if (!is_gun())
		{
			return;
		}

		memory::write<float>(held + offsets::baseprojectile_aimSwaySpeed, value);
	}

	void Item::set_aim_cone(float value)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return;
		}

		auto held = *reinterpret_cast<uintptr_t*>(entity + offsets::item_heldEntity);
		if (!(held))
		{
			return;
		}

		if (!is_gun())
		{
			return;
		}

		memory::write<float>(held + offsets::baseprojectile_aimCone, value);
	}

	void Item::set_aim_cone_curve_scale(float value)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return;
		}

		auto held = *reinterpret_cast<uintptr_t*>(entity + offsets::item_heldEntity);
		if (!(held))
		{
			return;
		}

		if (!is_gun())
		{
			return;
		}

		memory::write<float>(held + offsets::baseprojectile_aimCone, value);
	}

	void Item::set_aim_cone_penalty_max(float value)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return;
		}

		auto held = *reinterpret_cast<uintptr_t*>(entity + offsets::item_heldEntity);
		if (!(held))
		{
			return;
		}

		if (!is_gun())
		{
			return;
		}

		memory::write<float>(held + offsets::baseprojectile_aimConePenaltyMax, value);
	}


	void Item::set_aim_cone_penalty_per_shot(float value)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
		{
			return;
		}

		auto held = *reinterpret_cast<uintptr_t*>(entity + offsets::item_heldEntity);
		if (!(held))
		{
			return;
		}

		if (!is_gun())
		{
			return;
		}

		memory::write<float>(held + offsets::baseprojectile_aimconePenaltyPerShot, value);
	}

	void Item::set_hip_aim_cone(float value)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return;

		uintptr_t held = *reinterpret_cast<uintptr_t*>(entity + offsets::item_heldEntity);
		if (!held || !is_gun())
			return;

		*reinterpret_cast<float*>(held + offsets::baseprojectile_hipAimCone) *= value;
	}

	bool Item::is_gun()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return false;

		std::wstring item_name = get_name();

		if (item_name.find(xorstr_(L"rifle.")) != std::string::npos && item_name.find(xorstr_(L"ammo.")) == std::string::npos)
		{
			return true;
		}
		else if (item_name.find(xorstr_(L"pistol.")) != std::string::npos && item_name.find(xorstr_(L"ammo.")) == std::string::npos)
		{
			return true;
		}
		else if (item_name.find(xorstr_(L"bow.")) != std::string::npos && item_name.find(xorstr_(L"ammo.")) == std::string::npos)
		{
			return true;
		}
		else if (item_name.find(xorstr_(L"cross")) != std::string::npos && item_name.find(xorstr_(L"ammo.")) == std::string::npos)
		{
			return true;
		}
		else if (item_name.find(xorstr_(L"lmg.")) != std::string::npos && item_name.find(xorstr_(L"ammo.")) == std::string::npos)
		{
			return true;
		}
		else if (item_name.find(xorstr_(L"hmlmg")) != std::string::npos && item_name.find(xorstr_(L"ammo.")) == std::string::npos)
		{
			return true;
		}
		else if (item_name.find(xorstr_(L"shotgun.")) != std::string::npos && item_name.find(xorstr_(L"ammo.")) == std::string::npos)
		{
			return true;
		}
		else if (item_name.find(xorstr_(L"smg.")) != std::string::npos && item_name.find(xorstr_(L"ammo.")) == std::string::npos)
		{
			return true;
		}//hmlmg
		else
			return false;
	}

	uintptr_t ItemContainer::get_itemlist()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return 0;

		uintptr_t address = offsets::itemcontainer_itemList;

		if (!address)
			return 0;

		uintptr_t item_list = *reinterpret_cast<uintptr_t*>(entity + address);
		return item_list;
	}

	Item* ItemContainer::get_item(int id)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return nullptr;

		uintptr_t address = offsets::itemcontainer_itemList; //il2cpp::value(xorstr_("ItemContainer"), xorstr_("itemList"));
		if (!address)
			return nullptr;

		uintptr_t item_list = *reinterpret_cast<uintptr_t*>(entity + address);
		if (!(item_list))
			return nullptr;

		uintptr_t items = *reinterpret_cast<uintptr_t*>(item_list + 0x10);
		if (!(items))
			return nullptr;

		return *reinterpret_cast<Item**>(items + 0x20 + (id * 0x8));
	}

	int ItemDefinition::get_itemid()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return 0;

		return *reinterpret_cast<int*>(entity + offsets::itemdefinition_itemid);
	}

	std::wstring ItemDefinition::get_shortname()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return std::wstring();

		il2cpp::il2cppstring* entity_name = *reinterpret_cast<il2cpp::il2cppstring**>(entity + offsets::itemdefinition_shortname);
		if (!entity_name)
			return std::wstring();

		return entity_name->buffer;
	}

	ItemContainer* PlayerInventory::get_belt()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return nullptr;

		uintptr_t address = offsets::playerinventory_containerBelt; //il2cpp::value(xorstr_("PlayerInventory"), xorstr_("containerBelt"));
		if (!address)
			return nullptr;

		return *reinterpret_cast<ItemContainer**>(entity + address);
	}

	ItemContainer* PlayerInventory::get_wear()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return nullptr;

		uintptr_t address = offsets::playerinventory_containerWear; //il2cpp::value(xorstr_("PlayerInventory"), xorstr_("containerBelt"));
		if (!address)
			return nullptr;

		return *reinterpret_cast<ItemContainer**>(entity + address);
	}
}