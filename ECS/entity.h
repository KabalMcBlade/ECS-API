// Copyright (c) 2022-2025 Michele Condo'
// File: C:\Projects\ECS-API\ECS\entity.h
// Licensed under the MIT License. See LICENSE file in the project root for full license information.

#pragma once

#include "types.h"

#include <limits>
#include <utility>
#include <iostream>

ECS_NAMESPACE_BEGIN

static constexpr uint32 kMaxEntities = kMaxEntityIndex;

class ECS_API Entity
{
public:
    Entity() = delete;
    
	ECS_FORCE_INLINE bool operator==(const Entity _other) const
	{
		return m_id.m_index == _other.m_id.m_index && m_id.m_version == _other.m_id.m_version;
	}

	ECS_FORCE_INLINE bool operator!=(const Entity _other) const
	{
		return !((*this) == _other);
	}

	ECS_FORCE_INLINE bool IsValid() const
	{
		return m_id.m_index >= 0u && m_id.m_version > 0u;
	}

	ECS_FORCE_INLINE uint32 GetIndex() const
	{
		return static_cast<uint32>(m_id.m_index);
	}

	ECS_FORCE_INLINE uint32 GetVersion() const
	{
		return static_cast<uint32>(m_id.m_version);
	}

	Entity(uint16 _index, uint8 _version) : m_id({ _index, _version }) { }
    
    EntityId m_id;
    
    template<typename T, typename... Args>
    friend class IterateEntitiesWithAll;
    
    template<typename T, typename... Args>
    friend class IterateEntitiesWithAny;
    
    template<typename T, typename... Args>
    friend class IterateEntitiesWithNot;
};

static const ecs::Entity UnknowEntity = ecs::Entity(0, 0);

ECS_NAMESPACE_END
