// Copyright (c) 2022-2025 Michele Condo'
// File: C:\Projects\ECS-API\ECS\utility.h
// Licensed under the MIT License. See LICENSE file in the project root for full license information.

#pragma once

#include "types.h"


ECS_NAMESPACE_BEGIN

ECS_FORCE_INLINE constexpr const uint32 GetRequiredAmountOfUint64ToStoreBits(const uint32 _desiredBitsToStore) 
{
	return (_desiredBitsToStore + 63u) / 64u;
}

#if _WIN64
#include <intrin.h>
ECS_FORCE_INLINE uint64 CountTrailingZeros64(uint64 _bits)
{
	unsigned long index;
	_BitScanForward64(&index, _bits);
	return index;
}
#else
#include <x86intrin.h>
ECS_FORCE_INLINE uint64 CountTrailingZeros64(uint64 _bits)
{
	return static_cast<uint64>(__builtin_ctzll(_bits));
}
#endif

ECS_NAMESPACE_END
