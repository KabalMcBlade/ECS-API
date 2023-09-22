#pragma once

#include "types.h"
#include "entity.h"

#include <cassert>
#include <vector>


ECS_NAMESPACE_BEGIN

template<typename T, typename... Args>
class IterateEntitiesWithAny
{
public:
    IterateEntitiesWithAny() : m_entityIndex(0u) {}
    explicit IterateEntitiesWithAny(Entity _entity) : m_entityIndex(_entity.m_id.m_index) {}
    explicit IterateEntitiesWithAny(EntityId _entityId) : m_entityIndex(_entityId.m_index) {}
    explicit IterateEntitiesWithAny(uint16 _entityIndex) : m_entityIndex(_entityIndex) {}
    
    IterateEntitiesWithAny<T, Args...> begin();
    IterateEntitiesWithAny<T, Args...> end();
    
    void operator= (const IterateEntitiesWithAny<T, Args...>& _other);
    bool operator== (const IterateEntitiesWithAny<T, Args...>& _other);
    bool operator!= (const IterateEntitiesWithAny<T, Args...>& _other);
    Entity operator*();
    Entity operator->();
    
    IterateEntitiesWithAny<T, Args...> operator++();

private:
    uint16 m_entityIndex;
};


template<typename T, typename... Args>
IterateEntitiesWithAny<T, Args...> IterateEntitiesWithAny<T, Args...>::begin()
{
    uint16 i = 0;
    while(end().m_entityIndex > i)
    {
        if(!ComponentManager::HasAnyComponents<T, Args...>(i))
        {
            ++i;
            continue;
        }
        return IterateEntitiesWithAny<T, Args...>(i);
    }
    return end();
}

template<typename T, typename... Args>
IterateEntitiesWithAny<T, Args...> IterateEntitiesWithAny<T, Args...>::end()
{
    return IterateEntitiesWithAny<T, Args...>(EntityManager::m_totalEntityCreated);
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
    return EntityManager::GetEntity(m_entityIndex);
}

template<typename T, typename... Args>
Entity IterateEntitiesWithAny<T, Args...>::operator->()
{
    return EntityManager::GetEntity(m_entityIndex);
}

template<typename T, typename... Args>
IterateEntitiesWithAny<T, Args...> IterateEntitiesWithAny<T, Args...>::operator++()
{
    ++m_entityIndex;
    while(end().m_entityIndex > m_entityIndex)
    {
        if(ComponentManager::HasAnyComponents<T, Args...>(m_entityIndex))
        {
            return IterateEntitiesWithAny<T, Args...>(m_entityIndex);
        }
        ++m_entityIndex;
    }
    m_entityIndex = end().m_entityIndex;
    return end();
}

ECS_NAMESPACE_END