#pragma once

#include "types.h"
#include "entity.h"
#include "component_manager.h"

#include <cassert>
#include <vector>


ECS_NAMESPACE_BEGIN

template<typename T, typename... Args>
class IterateEntitiesWithAny
{
public:
    explicit IterateEntitiesWithAny(ComponentManager& _manager) : m_manager(_manager), m_entityIndex(0) {}
    explicit IterateEntitiesWithAny(ComponentManager& _manager, Entity _entity) : m_manager(_manager), m_entityIndex(_entity.m_id.m_index) {}
    explicit IterateEntitiesWithAny(ComponentManager& _manager, EntityId _entityId) : m_manager(_manager), m_entityIndex(_entityId.m_index) {}
    explicit IterateEntitiesWithAny(ComponentManager& _manager, uint16 _entityIndex) : m_manager(_manager), m_entityIndex(_entityIndex) {}
    
    IterateEntitiesWithAny<T, Args...> begin();
    IterateEntitiesWithAny<T, Args...> end();
    
    void operator= (const IterateEntitiesWithAny<T, Args...>& _other);
    bool operator== (const IterateEntitiesWithAny<T, Args...>& _other);
    bool operator!= (const IterateEntitiesWithAny<T, Args...>& _other);
    Entity operator*();
    Entity operator->();
    
    IterateEntitiesWithAny<T, Args...> operator++();

private:
    ComponentManager& m_manager;
    uint16 m_entityIndex;
};


template<typename T, typename... Args>
IterateEntitiesWithAny<T, Args...> IterateEntitiesWithAny<T, Args...>::begin()
{
    uint16 i = 0;
    while(end().m_entityIndex > i)
    {
        if(!m_manager.HasAnyComponents<T, Args...>(i))
        {
            ++i;
            continue;
        }
        return IterateEntitiesWithAny<T, Args...>(m_manager, i);
    }
    return end();
}

template<typename T, typename... Args>
IterateEntitiesWithAny<T, Args...> IterateEntitiesWithAny<T, Args...>::end()
{
    return IterateEntitiesWithAny<T, Args...>(m_manager, EntityManager::Instance().GetTotalEntityCreated());
}

template<typename T, typename... Args>
void IterateEntitiesWithAny<T, Args...>::operator= (const IterateEntitiesWithAny<T, Args...>& _other)
{
    m_entityIndex = _other.m_entityIndex;
}

template<typename T, typename... Args>
bool IterateEntitiesWithAny<T, Args...>::operator== (const IterateEntitiesWithAny<T, Args...>& _other)
{
    return m_entityIndex == _other.m_entityIndex;
}

template<typename T, typename... Args>
bool IterateEntitiesWithAny<T, Args...>::operator!= (const IterateEntitiesWithAny<T, Args...>& _other)
{
    return m_entityIndex != _other.m_entityIndex;
}

template<typename T, typename... Args>
Entity IterateEntitiesWithAny<T, Args...>::operator*()
{
    return EntityManager::Instance().GetEntity(m_entityIndex);
}

template<typename T, typename... Args>
Entity IterateEntitiesWithAny<T, Args...>::operator->()
{
    return EntityManager::Instance().GetEntity(m_entityIndex);
}

template<typename T, typename... Args>
IterateEntitiesWithAny<T, Args...> IterateEntitiesWithAny<T, Args...>::operator++()
{
    ++m_entityIndex;
    while(end().m_entityIndex > m_entityIndex)
    {
        if(m_manager.HasAnyComponents<T, Args...>(m_entityIndex))
        {
            return IterateEntitiesWithAny<T, Args...>(m_manager, m_entityIndex);
        }
        ++m_entityIndex;
    }
    m_entityIndex = end().m_entityIndex;
    return end();
}

ECS_NAMESPACE_END