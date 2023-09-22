#pragma once

#include "types.h"

#include <cstdint>

ECS_NAMESPACE_BEGIN

namespace _private
{
	// Source: https://gist.github.com/Lee-R/3839813
	constexpr uint32 fnv1a_32(char const* s, size_t count)
	{
		return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
	}
}

constexpr uint32 operator "" _Hash(char const* s, size_t count)
{
	return _private::fnv1a_32(s, count);
}

constexpr uint32 Hash(char const* s, size_t count)
{
	return _private::fnv1a_32(s, count);
}

ECS_NAMESPACE_END