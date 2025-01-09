// Copyright (c) 2022-2025 Michele Condo'
// File: C:\Projects\ECS-API\ECS\entity_collector.h
// Licensed under the MIT License. See LICENSE file in the project root for full license information.

#pragma once

#include "types.h"
#include "entity.h"
#include "entity_manager.h"
#include "component_manager.h"

#include <cassert>
#include <vector>

ECS_NAMESPACE_BEGIN

class ECS_API EntityCollector
{
public:
	EntityCollector() = delete;
	~EntityCollector() = delete;

	EntityCollector(const EntityCollector& _other) = delete;
	EntityCollector& operator=(const EntityCollector& _other) = delete;
	EntityCollector(EntityCollector&& _other) = delete;
	EntityCollector& operator=(EntityCollector&& _other) = delete;

	template<typename T, typename... Args>
	static std::vector<Entity> CollectEntitiesWithAll(const EntityManager& _entityManager, const ComponentManager& _componentManager);

	template<typename T, typename... Args>
	static std::vector<Entity> CollectEntitiesWithAny(const EntityManager& _entityManager, const ComponentManager& _componentManager);

	template<typename T, typename... Args>
	static std::vector<Entity> CollectEntitiesWithNot(const EntityManager& _entityManager, const ComponentManager& _componentManager);

	template<typename T, typename... Args>
	static std::unordered_map<typename T::FieldType, std::vector<Entity>>
	CollectAndGroupEntitiesWithAllByField(const EntityManager& _entityManager, ComponentManager& _componentManager, typename T::FieldType T::* _field);
	
	template<typename T, typename... Args>
	static std::unordered_map<typename T::FieldType, std::vector<Entity>>
	CollectAndGroupEntitiesWithAnyByField(const EntityManager& _entityManager, ComponentManager& _componentManager, typename T::FieldType T::* _field,
	std::vector<Entity>& _noGroupedEntities);
};

template<typename T, typename... Args>
std::vector<Entity> EntityCollector::CollectEntitiesWithAll(const EntityManager& _entityManager, const ComponentManager& _componentManager)
{
	std::vector<Entity> outEntities;

	uint16 index = 0;
	for (uint64 block : _entityManager.GetEntities())
	{
		while (block != 0)
		{
			const uint16 bitIndex = static_cast<uint16>(CountTrailingZeros64(block));
			block &= ~(1ull << bitIndex);

			const uint16 currentId = bitIndex + index;

			const bool has = _componentManager.HasComponents<T, Args...>(currentId);

			if (has)
			{
				outEntities.push_back(_entityManager.GetEntity(currentId));
			}
		}
		index += 64u;
	}

	return outEntities;
}

template<typename T, typename... Args>
std::vector<Entity> EntityCollector::CollectEntitiesWithAny(const EntityManager& _entityManager, const ComponentManager& _componentManager)
{
	std::vector<Entity> outEntities;

	uint16 index = 0;
	for (uint64 block : _entityManager.GetEntities())
	{
		while (block != 0)
		{
			const uint16 bitIndex = static_cast<uint16>(CountTrailingZeros64(block));
			block &= ~(1ull << bitIndex);

			const uint16 currentId = bitIndex + index;

			const bool hasAny = _componentManager.HasAnyComponents<T, Args...>(currentId);

			if (hasAny)
			{
				outEntities.push_back(_entityManager.GetEntity(currentId));
			}
		}
		index += 64u;
	}

	return outEntities;
}

template<typename T, typename... Args>
std::vector<Entity> EntityCollector::CollectEntitiesWithNot(const EntityManager& _entityManager, const ComponentManager& _componentManager)
{
	std::vector<Entity> outEntities;

	uint16 index = 0;
	for (uint64 block : _entityManager.GetEntities())
	{
		while (block != 0)
		{
			const uint16 bitIndex = static_cast<uint16>(CountTrailingZeros64(block));
			block &= ~(1ull << bitIndex);

			const uint16 currentId = bitIndex + index;

			const bool hasNot = _componentManager.HasNotComponents<T, Args...>(currentId);

			if (hasNot)
			{
				outEntities.push_back(_entityManager.GetEntity(currentId));
			}
		}
		index += 64u;
	}

	return outEntities;
}

template<typename T, typename... Args>
std::unordered_map<typename T::FieldType, std::vector<Entity>>
EntityCollector::CollectAndGroupEntitiesWithAllByField(const EntityManager& _entityManager, ComponentManager& _componentManager, typename T::FieldType T::* _field)
{
	std::unordered_map<typename T::FieldType, std::vector<Entity>> groupedEntities;

	uint16 index = 0;
	for (uint64 block : _entityManager.GetEntities())
	{
		while (block != 0)
		{
			const uint16 bitIndex = static_cast<uint16>(CountTrailingZeros64(block));
			block &= ~(1ull << bitIndex);

			const uint16 currentId = bitIndex + index;

			const bool has = _componentManager.HasComponents<T, Args...>(currentId);
			if (has)
			{
				const T& component = _componentManager.GetComponent<T>(currentId);
				typename T::FieldType fieldValue = component.*_field;

				groupedEntities[fieldValue].push_back(_entityManager.GetEntity(currentId));
			}
		}
		index += 64u;
	}

	return groupedEntities;
}


template<typename T, typename... Args>
std::unordered_map<typename T::FieldType, std::vector<Entity>>
EntityCollector::CollectAndGroupEntitiesWithAnyByField(const EntityManager& _entityManager, ComponentManager& _componentManager, typename T::FieldType T::* _field,
	std::vector<Entity>& _noGroupedEntities)
{
	std::unordered_map<typename T::FieldType, std::vector<Entity>> groupedEntities;

	uint16 index = 0;
	for (uint64 block : _entityManager.GetEntities())
	{
		while (block != 0)
		{
			const uint16 bitIndex = static_cast<uint16>(CountTrailingZeros64(block));
			block &= ~(1ull << bitIndex);

			const uint16 currentId = bitIndex + index;

			const bool hasAny = _componentManager.HasAnyComponents<T, Args...>(currentId);
			if (hasAny)
			{
				if (_componentManager.HasComponents<T>(currentId))
				{
					const T& component = _componentManager.GetComponent<T>(currentId);
					typename T::FieldType fieldValue = component.*_field;

					groupedEntities[fieldValue].push_back(_entityManager.GetEntity(currentId));
				}
				else
				{
					_noGroupedEntities.push_back(_entityManager.GetEntity(currentId));
				}
			}
		}
		index += 64u;
	}

	return groupedEntities;
}

ECS_NAMESPACE_END
