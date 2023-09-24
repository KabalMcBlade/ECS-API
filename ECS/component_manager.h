#pragma once

#include "types.h"
#include "entity.h"
#include "utility.h"
#include "hash.h"

#include <cassert>
#include <vector>
#include <cstring>
#include <algorithm>


ECS_NAMESPACE_BEGIN

namespace ComponentManager
{
    ECS_DLL extern std::vector<uint32> m_componentHashId;
    ECS_DLL extern uint16 m_maxEntities;

    ///

	inline void Create(uint16 _maxEntities, uint16 _maxComponents)
	{
		m_maxEntities = _maxEntities;
		assert(m_maxEntities <= kMaxEntities && "Max Entities exceeds the maximum number of entities!");

		m_componentHashId.reserve(_maxComponents);
	}

	inline void Destroy()
	{
		m_componentHashId.clear();
	}
	
	template <typename T>
    static void RegisterComponent();
    
    template <typename T>
    static void UnregisterComponent();
    
   	template <typename T>
    static bool IsComponentRegistered(); 
	
    template <typename T, typename... Args>
    static void AddComponent(const Entity _entity, const Args&... _args);
    
    template <typename T>
    static void RemoveComponent(const Entity _entity);
    
    template <typename T>
    static T& GetComponent(const Entity _entity);
    
    // HAS ALL
    template <typename T>
    static bool HasComponents(const Entity _entity);
    
    template <typename T1, typename T2, typename... Args>
    static bool HasComponents(const Entity _entity);

    template <typename T>
    static bool HasComponents(const EntityId _entityId);
    
    template <typename T1, typename T2, typename... Args>
    static bool HasComponents(const EntityId _entityId);
    
    template <typename T>
    static bool HasComponents(const uint16 _entityIndex);
    
    template <typename T1, typename T2, typename... Args>
    static bool HasComponents(const uint16 _entityIndex);

    // HAS ANY
    template <typename T>
    static bool HasAnyComponents(const Entity _entity);
    
    template <typename T1, typename T2, typename... Args>
    static bool HasAnyComponents(const Entity _entity);

    template <typename T>
    static bool HasAnyComponents(const EntityId _entityId);
    
    template <typename T1, typename T2, typename... Args>
    static bool HasAnyComponents(const EntityId _entityId);
    
    template <typename T>
    static bool HasAnyComponents(const uint16 _entityIndex);
    
    template <typename T1, typename T2, typename... Args>
    static bool HasAnyComponents(const uint16 _entityIndex);

    // HAS NOT
    template <typename T>
    static bool HasNotComponents(const Entity _entity);
    
    template <typename T1, typename T2, typename... Args>
    static bool HasNotComponents(const Entity _entity);

    template <typename T>
    static bool HasNotComponents(const EntityId _entityId);
    
    template <typename T1, typename T2, typename... Args>
    static bool HasNotComponents(const EntityId _entityId);
    
    template <typename T>
    static bool HasNotComponents(const uint16 _entityIndex);
    
    template <typename T1, typename T2, typename... Args>
    static bool HasNotComponents(const uint16 _entityIndex);
    

    ///

    template<typename T>
    ECS_DLL std::vector<T> m_components;

    template<typename T>
    ECS_DLL std::vector<uint64> m_componentsIndices;
};

template<typename T>
void ComponentManager::RegisterComponent()
{
	const char* typeName = typeid(T).name();
	const size_t typeNameSize = strlen(typeName);
	const uint32 hash = Hash(typeName, typeNameSize);

	assert(std::find(m_componentHashId.begin(), m_componentHashId.end(), hash) == m_componentHashId.end() && "Component already registered.");

    m_components<T>.resize(m_maxEntities);
    
    const uint32 capacity = GetRequiredAmountOfUint64ToStoreBits(m_maxEntities);
    m_componentsIndices<T>.resize(capacity, 0u);
    
	m_componentHashId.push_back(hash);
}

template <typename T>
void ComponentManager::UnregisterComponent()
{
  	const char* typeName = typeid(T).name();
	const size_t typeNameSize = strlen(typeName);
	const uint32 hash = Hash(typeName, typeNameSize);

	assert(std::find(m_componentHashId.begin(), m_componentHashId.end(), hash) != m_componentHashId.end() && "Component not registered.");

    m_components<T>.clear();
    m_componentsIndices<T>.clear();
    
    m_componentHashId.erase(std::remove_if(m_componentHashId.begin(), m_componentHashId.end(), [&](const uint32 _hash){ return _hash == hash; }), m_componentHashId.end());
}
    
template <typename T>
bool ComponentManager::IsComponentRegistered()
{
    const char* typeName = typeid(T).name();
	const size_t typeNameSize = strlen(typeName);
	const uint32 hash = Hash(typeName, typeNameSize);
	
    return std::find(m_componentHashId.begin(), m_componentHashId.end(), hash) != m_componentHashId.end();
}

template<typename T, typename... Args>
void ComponentManager::AddComponent(const Entity _entity, const Args&... _args)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");
    assert(!HasComponents<T>(_entity) && "Component already present in the entity.");

    // set bit
    m_componentsIndices<T>[_entity.m_id.m_index / 64u] |= (1ull << (_entity.m_id.m_index % 64u));
    
    m_components<T>[_entity.m_id.m_index] = T(_args...);
}


template<typename T>
void ComponentManager::RemoveComponent(const Entity _entity)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");
    assert(HasComponents<T>(_entity) && "Tried to remove non-existing component.");

    // clear bit
    m_componentsIndices<T>[_entity.m_id.m_index / 64u] &= ~(1ull << (_entity.m_id.m_index % 64u));
    
    m_components<T>[_entity.m_id.m_index] = T();
}


template<typename T>
T& ComponentManager::GetComponent(const Entity _entity)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");
    assert(HasComponents<T>(_entity) && "Tried to access non-existing component.");

    return m_components<T>[_entity.m_id.m_index];
}


// HAS ALL
template<typename T>
bool ComponentManager::HasComponents(const Entity _entity)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");

    return (m_componentsIndices<T>[_entity.m_id.m_index / 64u] & (1ull << (_entity.m_id.m_index % 64u))) != 0u;
}


template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasComponents(const Entity _entity)
{
    return HasComponents<T1>(_entity) && HasComponents<T2, Args...>(_entity);
}


template<typename T>
bool ComponentManager::HasComponents(const EntityId _entityId)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");

    return (m_componentsIndices<T>[_entityId.m_index / 64u] & (1ull << (_entityId.m_index % 64u))) != 0u;
}


template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasComponents(const EntityId _entityId)
{
    return HasComponents<T1>(_entityId) && HasComponents<T2, Args...>(_entityId);
}


template<typename T>
bool ComponentManager::HasComponents(const uint16 _entityIndex)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");

    return (m_componentsIndices<T>[_entityIndex / 64u] & (1ull << (_entityIndex % 64u))) != 0u;
}


template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasComponents(const uint16 _entityIndex)
{
    return HasComponents<T1>(_entityIndex) && HasComponents<T2, Args...>(_entityIndex);
}


// HAS ANY
template<typename T>
bool ComponentManager::HasAnyComponents(const Entity _entity)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");

    return (m_componentsIndices<T>[_entity.m_id.m_index / 64u] & (1ull << (_entity.m_id.m_index % 64u))) != 0u;
}


template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasAnyComponents(const Entity _entity)
{
    return HasAnyComponents<T1>(_entity) || HasAnyComponents<T2, Args...>(_entity);
}


template<typename T>
bool ComponentManager::HasAnyComponents(const EntityId _entityId)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");

    return (m_componentsIndices<T>[_entityId.m_index / 64u] & (1ull << (_entityId.m_index % 64u))) != 0u;
}


template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasAnyComponents(const EntityId _entityId)
{
    return HasAnyComponents<T1>(_entityId) || HasAnyComponents<T2, Args...>(_entityId);
}


template<typename T>
bool ComponentManager::HasAnyComponents(const uint16 _entityIndex)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");

    return (m_componentsIndices<T>[_entityIndex / 64u] & (1ull << (_entityIndex % 64u))) != 0u;
}


template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasAnyComponents(const uint16 _entityIndex)
{
    return HasAnyComponents<T1>(_entityIndex) || HasAnyComponents<T2, Args...>(_entityIndex);
}


// HAS NOT
template<typename T>
bool ComponentManager::HasNotComponents(const Entity _entity)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");

    return (m_componentsIndices<T>[_entity.m_id.m_index / 64u] & (1ull << (_entity.m_id.m_index % 64u))) == 0u;
}


template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasNotComponents(const Entity _entity)
{
    return HasNotComponents<T1>(_entity) && HasNotComponents<T2, Args...>(_entity);
}


template<typename T>
bool ComponentManager::HasNotComponents(const EntityId _entityId)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");

    return (m_componentsIndices<T>[_entityId.m_index / 64u] & (1ull << (_entityId.m_index % 64u))) == 0u;
}


template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasNotComponents(const EntityId _entityId)
{
    return HasNotComponents<T1>(_entityId) && HasNotComponents<T2, Args...>(_entityId);
}


template<typename T>
bool ComponentManager::HasNotComponents(const uint16 _entityIndex)
{
    assert(IsComponentRegistered<T>() && "Component has not been registered.");

    return (m_componentsIndices<T>[_entityIndex / 64u] & (1ull << (_entityIndex % 64u))) == 0u;
}


template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasNotComponents(const uint16 _entityIndex)
{
    return HasNotComponents<T1>(_entityIndex) && HasNotComponents<T2, Args...>(_entityIndex);
}

ECS_NAMESPACE_END