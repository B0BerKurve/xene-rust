#include "game/classes.hpp"
#include "game/offsets.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

namespace rust {
	long TeamMember::get_uid()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return long();

		return *reinterpret_cast<long*>(entity + 0x20);
	}

	long PlayerTeam::get_leader()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return 0;

		return *reinterpret_cast<long*>(entity + 0x20);
	}

	std::vector<long> PlayerTeam::get_members()
	{
		std::vector<long> member_vector;
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return member_vector;

		uintptr_t member_list = *reinterpret_cast<uintptr_t*>(entity + 0x28);
		if (!(member_list))
			return member_vector;

		uintptr_t members = *reinterpret_cast<uintptr_t*>(member_list + 0x10);
		if (!(members))
			return member_vector;

		int members_size = *reinterpret_cast<int*>(member_list + 0x18);

		if (members_size > 0)
		{
			for (auto i = 0; i < members_size; i++)
			{
				TeamMember* member = *reinterpret_cast<TeamMember**>(members + (0x20 + (i * 0x8)));
				if (!(reinterpret_cast<uintptr_t>(member)))
					continue;

				long member_uid = member->get_uid();
				if (member_uid > 0)
				{
					member_vector.push_back(member_uid);
				}
			}
		}

		return member_vector;
	}
}