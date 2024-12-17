#pragma once

#include "types.h"
#include "entity.h"

#include <cassert>
#include <vector>


ECS_NAMESPACE_BEGIN

template<typename T, typename... Args>
class IterateEntitiesWithAll
{
public:
    IterateEntitiesWithAll() : m_entityIndex(0u) {}
    explicit IterateEntitiesWithAll(Entity _entity) : m_entityIndex(_entity.m_id.m_index) {}
    explicit IterateEntitiesWithAll(EntityId _entityId) : m_entityIndex(_entityId.m_index) {}
    explicit IterateEntitiesWithAll(uint16 _entityIndex) : m_entityIndex(_entityIndex) {}
    
    IterateEntitiesWithAll<T, Args...> begin();
    IterateEntitiesWithAll<T, Args...> end();
    
    void operator= (const IterateEntitiesWithAll<T, Args...>& _other);
    bool operator== (const IterateEntitiesWithAll<T, Args...>& _other);
    bool operator!= (const IterateEntitiesWithAll<T, Args...>& _other);
    Entity operator*();
    Entity operator->();
    
    IterateEntitiesWithAll<T, Args...> operator++();

private:
    uint16 m_entityIndex;
};


template<typename T, typename... Args>
IterateEntitiesWithAll<T, Args...> IterateEntitiesWithAll<T, Args...>::begin()
{
    uint16 i = 0;
    while(end().m_entityIndex > i)
    {
        if(!ComponentManager::Instance().HasComponents<T, Args...>(i))
        {
            ++i;
            continue;
        }
        return IterateEntitiesWithAll<T, Args...>(i);
    }
    return end();
}

template<typename T, typename... Args>
IterateEntitiesWithAll<T, Args...> IterateEntitiesWithAll<T, Args...>::end()
{
    return IterateEntitiesWithAll<T, Args...>(EntityManager::Instance().GetTotalEntityCreated());
}

template<typename T, typename... Args>
void IterateEntitiesWithAll<T, Args...>::operator= (const IterateEntitiesWithAll<T, Args...>& _other)
{
    m_entityIndex = _other.m_entityIndex;
}

template<typename T, typename... Args>
bool IterateEntitiesWithAll<T, Args...>::operator== (const IterateEntitiesWithAll<T, Args...>& _other)
{
    return m_entityIndex == _other.m_entityIndex;
}

template<typename T, typename... Args>
bool IterateEntitiesWithAll<T, Args...>::operator!= (const IterateEntitiesWithAll<T, Args...>& _other)
{
    return m_entityIndex != _other.m_entityIndex;
}

template<typename T, typename... Args>
Entity IterateEntitiesWithAll<T, Args...>::operator*()
{
    return EntityManager::Instance().GetEntity(m_entityIndex);
}

template<typename T, typename... Args>
Entity IterateEntitiesWithAll<T, Args...>::operator->()
{
    return EntityManager::Instance().GetEntity(m_entityIndex);
}

template<typename T, typename... Args>
IterateEntitiesWithAll<T, Args...> IterateEntitiesWithAll<T, Args...>::operator++()
{
    ++m_entityIndex;
    while(end().m_entityIndex > m_entityIndex)
    {
        if(ComponentManager::Instance().HasComponents<T, Args...>(m_entityIndex))
        {
            return IterateEntitiesWithAll<T, Args...>(m_entityIndex);
        }
        ++m_entityIndex;
    }
    m_entityIndex = end().m_entityIndex;
    return end();
}

/*
NOTE: is possible to iterate all and check afterward in this way:
for(auto current : IterateEntitiesWithAll<void>())
{
    if(current.HasComponents<Position>())
    {
        // yada yada
    }
}
*/

ECS_NAMESPACE_END