// Copyright (c) 2022-2025 Michele Condo'
// File: C:\Projects\ECS-API\ECS\iterate_entities_with_not.h
// Licensed under the MIT License. See LICENSE file in the project root for full license information.

#pragma once

#include "types.h"
#include "entity.h"
#include "entity_manager.h"
#include "component_manager.h"

#include <cassert>
#include <vector>


ECS_NAMESPACE_BEGIN

template<typename T, typename... Args>
class IterateEntitiesWithNot
{
public:
    explicit IterateEntitiesWithNot(EntityManager& _entityManager, ComponentManager& _componentManager) : m_entityManager(_entityManager), m_componentManager(_componentManager), m_entityIndex(0) {}
    explicit IterateEntitiesWithNot(EntityManager& _entityManager, ComponentManager& _componentManager, Entity _entity) : m_entityManager(_entityManager), m_componentManager(_componentManager), m_entityIndex(_entity.m_id.m_index) {}
    explicit IterateEntitiesWithNot(EntityManager& _entityManager, ComponentManager& _componentManager, EntityId _entityId) : m_entityManager(_entityManager), m_componentManager(_componentManager), m_entityIndex(_entityId.m_index) {}
    explicit IterateEntitiesWithNot(EntityManager& _entityManager, ComponentManager& _componentManager, uint16 _entityIndex) : m_entityManager(_entityManager), m_componentManager(_componentManager), m_entityIndex(_entityIndex) {}
    
    IterateEntitiesWithNot<T, Args...> begin();
    IterateEntitiesWithNot<T, Args...> end();
    
    void operator= (const IterateEntitiesWithNot<T, Args...>& _other);
    bool operator== (const IterateEntitiesWithNot<T, Args...>& _other);
    bool operator!= (const IterateEntitiesWithNot<T, Args...>& _other);
    Entity operator*();
    Entity operator->();
    
    IterateEntitiesWithNot<T, Args...> operator++();

private:
	EntityManager& m_entityManager;
	ComponentManager& m_componentManager;
    uint16 m_entityIndex;
};


template<typename T, typename... Args>
IterateEntitiesWithNot<T, Args...> IterateEntitiesWithNot<T, Args...>::begin()
{
    uint16 i = 0;
    while(end().m_entityIndex > i)
    {
        if(!m_componentManager.HasNotComponents<T, Args...>(i))
        {
            ++i;
            continue;
        }
        return IterateEntitiesWithNot<T, Args...>(m_entityManager, m_componentManager, i);
    }
    return end();
}

template<typename T, typename... Args>
IterateEntitiesWithNot<T, Args...> IterateEntitiesWithNot<T, Args...>::end()
{
    return IterateEntitiesWithNot<T, Args...>(m_entityManager, m_componentManager, m_entityManager.GetTotalEntityCreated());
}

template<typename T, typename... Args>
void IterateEntitiesWithNot<T, Args...>::operator= (const IterateEntitiesWithNot<T, Args...>& _other)
{
    m_entityIndex = _other.m_entityIndex;
}

template<typename T, typename... Args>
bool IterateEntitiesWithNot<T, Args...>::operator== (const IterateEntitiesWithNot<T, Args...>& _other)
{
    return m_entityIndex == _other.m_entityIndex;
}

template<typename T, typename... Args>
bool IterateEntitiesWithNot<T, Args...>::operator!= (const IterateEntitiesWithNot<T, Args...>& _other)
{
    return m_entityIndex != _other.m_entityIndex;
}

template<typename T, typename... Args>
Entity IterateEntitiesWithNot<T, Args...>::operator*()
{
    return m_entityManager.GetEntity(m_entityIndex);
}

template<typename T, typename... Args>
Entity IterateEntitiesWithNot<T, Args...>::operator->()
{
    return m_entityManager.GetEntity(m_entityIndex);
}

template<typename T, typename... Args>
IterateEntitiesWithNot<T, Args...> IterateEntitiesWithNot<T, Args...>::operator++()
{
    ++m_entityIndex;
    while(end().m_entityIndex > m_entityIndex)
    {
        if(m_componentManager.HasNotComponents<T, Args...>(m_entityIndex))
        {
            return IterateEntitiesWithNot<T, Args...>(m_entityManager, m_componentManager, m_entityIndex);
        }
        ++m_entityIndex;
    }
    m_entityIndex = end().m_entityIndex;
    return end();
}

ECS_NAMESPACE_END