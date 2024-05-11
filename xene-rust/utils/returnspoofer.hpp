#pragma once
#include <type_traits>

// https://github.com/juniorjacob/ret_addr_spoofer



template <typename result, typename... arguments>
__forceinline static result SpoofReturn(result(*fn)(arguments...), arguments... args)
{
	return fn(args...);

	//todo add a real spoof caller
}
