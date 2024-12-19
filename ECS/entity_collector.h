#pragma once

#include "types.h"
#include "entity.h"
#include "entity_manager.h"
#include "component_manager.h"

#include <cassert>
#include <vector>

ECS_NAMESPACE_BEGIN

class EntityCollector
{
public:
    EntityCollector() = delete;
	~EntityCollector() = delete;
	
	EntityCollector(const EntityCollector& _other) = delete;
	EntityCollector& operator=(const EntityCollector& _other) = delete;
	EntityCollector(EntityCollector&& _other) = delete;
	EntityCollector& operator=(EntityCollector&& _other) = delete;
	
    template<typename T, typename... Args>
    static void CollectEntitiesWithAll(const EntityManager& _entityManager, const ComponentManager& _componentManager, std::vector<Entity>& _outEntities);
    
    template<typename T, typename... Args>
    static void CollectEntitiesWithAny(const EntityManager& _entityManager, const ComponentManager& _componentManager, std::vector<Entity>& _outEntities);
    
    template<typename T, typename... Args>
    static void CollectEntitiesWithNot(const EntityManager& _entityManager, const ComponentManager& _componentManager, std::vector<Entity>& _outEntities);
};

template<typename T, typename... Args>
void EntityCollector::CollectEntitiesWithAll(const EntityManager& _entityManager, const ComponentManager& _componentManager, std::vector<Entity>& _outEntities)
{
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
		        _outEntities.push_back(_entityManager.GetEntity(currentId));
			}
		}
		index += 64u;
	}
}

template<typename T, typename... Args>
void EntityCollector::CollectEntitiesWithAny(const EntityManager& _entityManager, const ComponentManager& _componentManager, std::vector<Entity>& _outEntities)
{
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
		        _outEntities.push_back(_entityManager.GetEntity(currentId));
			}
		}
		index += 64u;
	}
}

template<typename T, typename... Args>
void EntityCollector::CollectEntitiesWithNot(const EntityManager& _entityManager, const ComponentManager& _componentManager, std::vector<Entity>& _outEntities)
{
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
		        _outEntities.push_back(_entityManager.GetEntity(currentId));
			}
		}
		index += 64u;
	}
}

ECS_NAMESPACE_END