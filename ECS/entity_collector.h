#pragma once

#include "types.h"
#include "entity.h"

#include <cassert>
#include <vector>

ECS_NAMESPACE_BEGIN

class EntityCollector
{
public:
	explicit EntityCollector(const ComponentManager& _componentManager, const EntityManager& _entityManager)
		: m_componentManager(_componentManager), m_entityManager(_entityManager) {}

	~EntityCollector() = default;
	
	EntityCollector(const EntityCollector& _other) = delete;
	EntityCollector& operator=(const EntityCollector& _other) = delete;
	EntityCollector(EntityCollector&& _other) = delete;
	EntityCollector& operator=(EntityCollector&& _other) = delete;
	
    template<typename T, typename... Args>
    void CollectEntitiesWithAll(std::vector<Entity>& _outEntities);
    
    template<typename T, typename... Args>
    void CollectEntitiesWithAny(std::vector<Entity>& _outEntities);
    
    template<typename T, typename... Args>
    void CollectEntitiesWithNot(std::vector<Entity>& _outEntities);

private:
	const ComponentManager& m_componentManager;
	const EntityManager& m_entityManager;
};

template<typename T, typename... Args>
void EntityCollector::CollectEntitiesWithAll(std::vector<Entity>& _outEntities)
{
    uint16 index = 0;
	for (uint64 block : m_entityManager.GetEntities())
	{
		while (block != 0)
		{
			const uint16 bitIndex = static_cast<uint16>(CountTrailingZeros64(block));
			block &= ~(1ull << bitIndex);
			
			const uint16 currentId = bitIndex + index;
			
		    const bool has = m_componentManager.HasComponents<T, Args...>(currentId);
		    
		    if (has)
			{
		        _outEntities.push_back(m_entityManager.GetEntity(currentId));
			}
		}
		index += 64u;
	}
}

template<typename T, typename... Args>
void EntityCollector::CollectEntitiesWithAny(std::vector<Entity>& _outEntities)
{
    uint16 index = 0;
	for (uint64 block : m_entityManager.GetEntities())
	{
		while (block != 0)
		{
			const uint16 bitIndex = static_cast<uint16>(CountTrailingZeros64(block));
			block &= ~(1ull << bitIndex);
			
			const uint16 currentId = bitIndex + index;
			
		    const bool hasAny = m_componentManager.HasAnyComponents<T, Args...>(currentId);
		    
		    if (hasAny)
			{
		        _outEntities.push_back(m_entityManager.GetEntity(currentId));
			}
		}
		index += 64u;
	}
}

template<typename T, typename... Args>
void EntityCollector::CollectEntitiesWithNot(std::vector<Entity>& _outEntities)
{
    uint16 index = 0;
	for (uint64 block : m_entityManager.GetEntities())
	{
		while (block != 0)
		{
			const uint16 bitIndex = static_cast<uint16>(CountTrailingZeros64(block));
			block &= ~(1ull << bitIndex);
			
			const uint16 currentId = bitIndex + index;
			
		    const bool hasNot = m_componentManager.HasNotComponents<T, Args...>(currentId);
		    
		    if (hasNot)
			{
		        _outEntities.push_back(m_entityManager.GetEntity(currentId));
			}
		}
		index += 64u;
	}
}

ECS_NAMESPACE_END