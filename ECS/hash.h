// Copyright (c) 2022-2025 Michele Condo'
// File: C:\Projects\ECS-API\ECS\hash.h
// Licensed under the MIT License. See LICENSE file in the project root for full license information.

#pragma once

#include "types.h"

#include <cstdint>

ECS_NAMESPACE_BEGIN

namespace _private
{
	// Source: https://gist.github.com/Lee-R/3839813
	ECS_FORCE_INLINE constexpr uint32 fnv1a_32(char const* s, size_t count)
	{
		return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u; // NOLINT (hicpp-signed-bitwise)
	}
}

ECS_FORCE_INLINE constexpr uint32 operator "" _Hash(char const* s, size_t count)
{
	return _private::fnv1a_32(s, count);
}

ECS_FORCE_INLINE constexpr uint32 Hash(char const* s, size_t count)
{
	return _private::fnv1a_32(s, count);
}

ECS_NAMESPACE_END