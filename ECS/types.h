// Copyright (c) 2022-2025 Michele Condo'
// File: C:\Projects\ECS-API\ECS\types.h
// Licensed under the MIT License. See LICENSE file in the project root for full license information.

#pragma once

#include <cassert>

#define ECS_NAMESPACE_BEGIN namespace ecs {
#define ECS_NAMESPACE_END };  


// Check windows
#if _WIN32
#define ECS_FORCE_INLINE __forceinline
#endif

// Check GCC
#if __GNUC__
#define ECS_FORCE_INLINE __attribute__((always_inline)) inline
#endif


#if defined(ECS_DLL_EXPORT)             // When exporting from the DLL
#define ECS_API __declspec(dllexport)
#elif defined(ECS_DLL_IMPORT)           // When importing to the DLL
#define ECS_API __declspec(dllimport)
#else                                   // When using the static library
#define ECS_API
#endif


ECS_NAMESPACE_BEGIN

/// <summary>
/// TYPES
/// </summary>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef signed char int8;
typedef signed short int16;
typedef signed int int32;

typedef float float32;
typedef double float64;

#if _WIN32 || _WIN64
typedef unsigned long long uint64;
typedef long long int64;
#if _WIN64
typedef __int64 intptr;
typedef unsigned long long uintptr;
#else
typedef int intptr;
typedef unsigned int uintptr;
#endif
#else
typedef unsigned long uint64;
typedef long int64;
typedef unsigned long uintptr;
typedef int64 intptr;
#endif

typedef intptr ptrdiff;



/// <summary>
/// INTERNAL DATA
/// </summary>

static constexpr uint32 kMaxEntityIndex = 16777215u;

struct ECS_API _EntityID
{
    uint32 m_index : 24;
    uint8 m_version : 8;
    
    _EntityID(uint32 _index, uint8 _version) : m_index(_index), m_version(_version) 
    {
        assert(_index < kMaxEntityIndex && "Index out of range! Max index allowed is 16777214");
    }
};

typedef _EntityID EntityId;


ECS_NAMESPACE_END
