#pragma once

namespace hookmanager
{
	void hook(const MethodInfo* target, PVOID replacement, PVOID original)
	{
		if (!target) return;

		auto orig = *reinterpret_cast<std::uint64_t*>(reinterpret_cast<std::uint64_t>(target));
		if (orig != reinterpret_cast<std::uint64_t>(target)) *reinterpret_cast<std::uint64_t*>(original) = orig;

		*reinterpret_cast<PVOID*>(reinterpret_cast<std::uint64_t>(target)) = replacement;
	}

	void unhook(const MethodInfo* target, PVOID original)
	{
		*reinterpret_cast<PVOID*>(reinterpret_cast<std::uint64_t>(target)) = original;
	}
}