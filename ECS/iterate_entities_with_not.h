#pragma once

#include "types.h"
#include "entity.h"

#include <cassert>
#include <vector>


ECS_NAMESPACE_BEGIN

template<typename T, typename... Args>
class IterateEntitiesWithNot
{
public:
    IterateEntitiesWithNot() : m_entityIndex(0u) {}
    explicit IterateEntitiesWithNot(Entity _entity) : m_entityIndex(_entity.m_id.m_index) {}
    explicit IterateEntitiesWithNot(EntityId _entityId) : m_entityIndex(_entityId.m_index) {}
    explicit IterateEntitiesWithNot(uint16 _entityIndex) : m_entityIndex(_entityIndex) {}
    
    IterateEntitiesWithNot<T, Args...> begin();
    IterateEntitiesWithNot<T, Args...> end();
    
    void operator= (const IterateEntitiesWithNot<T, Args...>& _other);
    bool operator== (const IterateEntitiesWithNot<T, Args...>& _other);
    bool operator!= (const IterateEntitiesWithNot<T, Args...>& _other);
    Entity operator*();
    Entity operator->();
    
    IterateEntitiesWithNot<T, Args...> operator++();

private:
    uint16 m_entityIndex;
};


template<typename T, typename... Args>
IterateEntitiesWithNot<T, Args...> IterateEntitiesWithNot<T, Args...>::begin()
{
    uint16 i = 0;
    while(end().m_entityIndex > i)
    {
        if(!ComponentManager::Instance().HasNotComponents<T, Args...>(i))
        {
            ++i;
            continue;
        }
        return IterateEntitiesWithNot<T, Args...>(i);
    }
    return end();
}

template<typename T, typename... Args>
IterateEntitiesWithNot<T, Args...> IterateEntitiesWithNot<T, Args...>::end()
{
    return IterateEntitiesWithNot<T, Args...>(EntityManager::Instance().GetTotalEntityCreated());
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
    return EntityManager::Instance().GetEntity(m_entityIndex);
}

template<typename T, typename... Args>
Entity IterateEntitiesWithNot<T, Args...>::operator->()
{
    return EntityManager::Instance().GetEntity(m_entityIndex);
}

template<typename T, typename... Args>
IterateEntitiesWithNot<T, Args...> IterateEntitiesWithNot<T, Args...>::operator++()
{
    ++m_entityIndex;
    while(end().m_entityIndex > m_entityIndex)
    {
        if(ComponentManager::Instance().HasNotComponents<T, Args...>(m_entityIndex))
        {
            return IterateEntitiesWithNot<T, Args...>(m_entityIndex);
        }
        ++m_entityIndex;
    }
    m_entityIndex = end().m_entityIndex;
    return end();
}

ECS_NAMESPACE_END