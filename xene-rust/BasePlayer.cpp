#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	bool BasePlayer::is_mounted()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return false;

		uintptr_t method = offsets::ismounted;
		if (!method)
			return false;

		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
			return false;

		auto func = reinterpret_cast<bool(*)(uintptr_t)>(method_ptr);
		return SpoofReturn(func, entity);
	}

	std::wstring BasePlayer::get_username()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return std::wstring();

		uintptr_t address = offsets::_displayName;

		il2cpp::il2cppstring* player_name = *reinterpret_cast<il2cpp::il2cppstring**>(entity + address);
		if (!(reinterpret_cast<uintptr_t>(player_name)))
			return std::wstring();

		return player_name->buffer;
	}

	long BasePlayer::get_uid()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return long();

		uintptr_t address = offsets::userID;
		if (!address)
			return long();

		return *reinterpret_cast<long*>(entity + address);
	}

	int BasePlayer::get_active_weapon_id()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return int();

		return *reinterpret_cast<int*>(entity + offsets::clActiveItem);
	}

	PlayerWalkMovement* BasePlayer::get_movement()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return nullptr;

		return *reinterpret_cast<PlayerWalkMovement**>(entity + offsets::movement);
	}

	PlayerInput* BasePlayer::get_input()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return nullptr;

		uintptr_t address = offsets::input;
		if (!address)
			return nullptr;

		return *reinterpret_cast<PlayerInput**>(entity + address);
	}

	PlayerInventory* BasePlayer::get_inventory()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return nullptr;

		uintptr_t address = offsets::inventory;
		if (!address)
			return nullptr;

		return *reinterpret_cast<PlayerInventory**>(entity + address);
	}

	PlayerTeam* BasePlayer::get_team()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return nullptr;

		uintptr_t address = offsets::clientTeam;
		if (!address)
			return nullptr;

		return *reinterpret_cast<PlayerTeam**>(entity + address);
	}

	ULONG BasePlayer::get_team_id()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return 8766743523;

		uintptr_t address = offsets::currentTeam;
		if (!address)
			return 8766743523;


		return *reinterpret_cast<ULONG*>(entity + address);
	}

	ModelState* BasePlayer::get_modelstate()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return nullptr;

		uintptr_t address = offsets::modelstate;
		if (!address)
			return nullptr;

		return *reinterpret_cast<ModelState**>(entity + address);
	}

	PlayerModel* BasePlayer::get_model()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return nullptr;

		uintptr_t address = offsets::player_model;
		if (!address)
			return nullptr;

		return *reinterpret_cast<PlayerModel**>(entity + address);
	}

	PlayerEyes* BasePlayer::get_eyes()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return nullptr;

		uintptr_t address = offsets::player_eyes;
		if (!address)
			return nullptr;

		return *reinterpret_cast<PlayerEyes**>(entity + address);
	}

	Item* BasePlayer::get_held_item()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return nullptr;

		int active_item_id = get_active_weapon_id();
		if (!active_item_id || active_item_id == NULL)
		{
			return nullptr;
		}

		for (int i = 0; i < 6; i++)
		{
			PlayerInventory* inventory = get_inventory();
			if (!(reinterpret_cast<uintptr_t>(inventory)))
				continue;

			ItemContainer* belt = inventory->get_belt();
			if (!(reinterpret_cast<uintptr_t>(belt)))
				continue;

			Item* weapon = belt->get_item(i);
			if (!(reinterpret_cast<uintptr_t>(weapon)))
				continue;

			if (active_item_id == weapon->get_uid())
			{
				return weapon;
			}
		}

		return nullptr;
	}

	bool BasePlayer::is_knocked()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return bool();

		uintptr_t address1 = offsets::playerFlags;

		int flags = *reinterpret_cast<int*>(entity + address1);

		return flags & 64;
	}

	bool BasePlayer::is_sleeping()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return bool();

		uintptr_t address1 = offsets::playerFlags;

		int flags = *reinterpret_cast<int*>(entity + address1);

		return flags & 16;
	}

	bool BasePlayer::is_visible(Vector3 end)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return bool();

		return functions::is_visible(get_bone_position(eyeTranform), end);
	}

	Vector3 BasePlayer::get_bone_position(int bone)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return Vector3();

		uintptr_t address1 = offsets::model;
		if (!address1)
			return Vector3();

		uintptr_t player_model = *reinterpret_cast<uintptr_t*>(entity + address1); //BaseEntity -> model.
		if (!(player_model))
			return Vector3();

		uintptr_t address2 = offsets::boneTransforms;

		uintptr_t bone_transforms = *reinterpret_cast<uintptr_t*>(player_model + address2);
		if (!(bone_transforms))
			return Vector3();

		Transform* entity_bone = *reinterpret_cast<Transform**>(bone_transforms + (0x20 + (bone * 0x8)));
		if (!entity_bone)
			return Vector3();

		return entity_bone->get_position();
	}

	Transform* BasePlayer::get_bone_transform(int bone)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return nullptr;

		uintptr_t address1 = offsets::model;
		if (!address1)
			return nullptr;

		uintptr_t player_model = *reinterpret_cast<uintptr_t*>(entity + address1); //BaseEntity -> model.
		if (!player_model)
			return nullptr;

		uintptr_t address2 = offsets::boneTransforms;

		uintptr_t bone_transforms = *reinterpret_cast<uintptr_t*>(player_model + address2);
		if (!(bone_transforms))
			return nullptr;

		Transform* entity_bone = *reinterpret_cast<Transform**>(bone_transforms + (0x20 + (bone * 0x8)));
		if (!entity_bone)
			return nullptr;

		return entity_bone;
	}

	void BasePlayer::add_flag(int flag)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!(entity))
			return;

		auto address = offsets::playerFlags;

		int flags = *reinterpret_cast<int*>(entity + address);

		flags |= flag;

		memory::write<int>(entity + address, flags);
		return;
	}

	bool BasePlayer::has_flag(int flag)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return false;

		auto address = offsets::playerFlags;

		int flags = *reinterpret_cast<int*>(entity + address);

		return (flags & flag) != 0;
	}

	bool BasePlayer::is_bot()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return false;

		uintptr_t method = offsets::isbot; //il2cpp::method(xorstr_("Mathf"), xorstr_("Clamp"), 3, xorstr_("UnityEngine"));
		if (!method)
		{
			return false;
		}
		uintptr_t method_ptr = *reinterpret_cast<uintptr_t*>(method);
		if (!method_ptr)
		{
			return false;
		}
		auto isBot = reinterpret_cast<bool(*)(uintptr_t)>(method_ptr);
		return SpoofReturn(isBot, entity);
	}

	std::vector<Item*> BasePlayer::get_clothing_items()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return std::vector<Item*>();

		std::vector<Item*> temp;

		PlayerInventory* inventory = get_inventory();
		if (reinterpret_cast<uintptr_t>(inventory)) {
			ItemContainer* wear = inventory->get_wear();
			if (wear) {
				uintptr_t itemList = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(wear) + 0x40);
				if (itemList) {
					int itemsize = *reinterpret_cast<int*>(itemList + 0x18);
					for (int i = 0; i < itemsize; i++)
					{
						Item* clothing = wear->get_item(i);
						if (!(reinterpret_cast<uintptr_t>(clothing)))
							continue;

						temp.push_back(clothing);
						if (temp.empty())
							continue;
					}
				}
				else {
					std::cout << "failed itemlist" << std::endl;
				}
			}
		}

		return temp;
	}

	std::vector<Item*> BasePlayer::get_hotbar_items()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);
		if (!entity)
			return std::vector<Item*>();

		std::vector<Item*> temp;

		PlayerInventory* inventory = get_inventory();
		if (reinterpret_cast<uintptr_t>(inventory)) {
			ItemContainer* belt = inventory->get_belt();
			if (belt) {
				uintptr_t itemList = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(belt) + 0x40);
				if (itemList) {
					int itemsize = *reinterpret_cast<int*>(itemList + 0x18);
					for (int i = 0; i < itemsize; i++)
					{
						Item* weapon = belt->get_item(i);
						if (!(reinterpret_cast<uintptr_t>(weapon)))
							continue;

						temp.push_back(weapon);
						if (temp.empty())
							continue;
					}
				}
				else {
					std::cout << "failed itemlist" << std::endl;
				}
			}
		}

		return temp;
	}


}