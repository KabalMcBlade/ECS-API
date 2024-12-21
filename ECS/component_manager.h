#pragma once

#include "types.h"
#include "entity.h"
#include "utility.h"
#include "hash.h"

#include <cassert>
#include <vector>
#include <cstring>
#include <algorithm>
#include <unordered_map>
#include <typeinfo>

// for now, for the warning of exporting std classes
#pragma warning(disable : 4251)

ECS_NAMESPACE_BEGIN

class ECS_DLL ComponentManager
{
public:
	static ComponentManager& Instance()
	{
		static ComponentManager instance;
		return instance;
	}

	void Create(uint16 _maxEntities, uint16 _maxComponents);
	void Destroy();

	template <typename T>
	void RegisterComponent();

	template <typename T>
	void UnregisterComponent();

	template <typename T>
	bool IsComponentRegistered() const;

	template <typename T, typename... Args>
	void AddComponent(const Entity _entity, const Args&... _args);

	template <typename T>
	void RemoveComponent(const Entity _entity);

	template <typename T>
	T& GetComponent(const Entity _entity);

	// HAS ALL
	template <typename T>
	bool HasComponents(const Entity _entity) const;

	template <typename T1, typename T2, typename... Args>
	bool HasComponents(const Entity _entity) const;

	template <typename T>
	bool HasComponents(const EntityId _entityId) const;

	template <typename T1, typename T2, typename... Args>
	bool HasComponents(const EntityId _entityId) const;

	template <typename T>
	bool HasComponents(const uint16 _entityIndex) const;

	template <typename T1, typename T2, typename... Args>
	bool HasComponents(const uint16 _entityIndex) const;

	// HAS ANY
	template <typename T>
	bool HasAnyComponents(const Entity _entity) const;

	template <typename T1, typename T2, typename... Args>
	bool HasAnyComponents(const Entity _entity) const;

	template <typename T>
	bool HasAnyComponents(const EntityId _entityId) const;

	template <typename T1, typename T2, typename... Args>
	bool HasAnyComponents(const EntityId _entityId) const;

	template <typename T>
	bool HasAnyComponents(const uint16 _entityIndex) const;

	template <typename T1, typename T2, typename... Args>
	bool HasAnyComponents(const uint16 _entityIndex) const;

	// HAS NOT
	template <typename T>
	bool HasNotComponents(const Entity _entity) const;

	template <typename T1, typename T2, typename... Args>
	bool HasNotComponents(const Entity _entity) const;

	template <typename T>
	bool HasNotComponents(const EntityId _entityId) const;

	template <typename T1, typename T2, typename... Args>
	bool HasNotComponents(const EntityId _entityId) const;

	template <typename T>
	bool HasNotComponents(const uint16 _entityIndex) const;

	template <typename T1, typename T2, typename... Args>
	bool HasNotComponents(const uint16 _entityIndex) const;

private:
	ComponentManager() = default;
	~ComponentManager() = default;

	ComponentManager(const ComponentManager&) = delete;
	ComponentManager& operator=(const ComponentManager&) = delete;

	std::unordered_map<uint32, std::shared_ptr<void>> m_components;
	std::unordered_map<uint32, std::shared_ptr<std::vector<uint64>>> m_componentIndices;
	std::vector<uint32> m_componentHashId;
	uint16 m_maxEntities = 0;
};


ECS_DLL ComponentManager& GetComponentManager();


template<typename T>
void ComponentManager::RegisterComponent()
{
    const char* typeName = typeid(T).name();
	const uint32 hash = Hash(typeName, std::strlen(typeName));

    assert(m_components.find(hash) == m_components.end() && "Component already registered!");

    m_components[hash] = std::make_shared<std::vector<T>>(m_maxEntities);
    m_componentIndices[hash] = std::make_shared<std::vector<uint64>>(GetRequiredAmountOfUint64ToStoreBits(m_maxEntities), 0u);
    m_componentHashId.push_back(hash);
}

template <typename T>
void ComponentManager::UnregisterComponent()
{
	const char* typeName = typeid(T).name();
	const uint32 hash = Hash(typeName, std::strlen(typeName));

	assert(m_components.find(hash) != m_components.end() && "Component not registered!");

	m_components.erase(hash);
	m_componentIndices.erase(hash);
	m_componentHashId.erase(std::remove(m_componentHashId.begin(), m_componentHashId.end(), hash), m_componentHashId.end());
}
    
template <typename T>
bool ComponentManager::IsComponentRegistered() const
{
	const char* typeName = typeid(T).name();
	const uint32 hash = Hash(typeName, std::strlen(typeName));
	return m_components.find(hash) != m_components.end();
}

template<typename T, typename... Args>
void ComponentManager::AddComponent(const Entity _entity, const Args&... _args)
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	const uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& components = *std::static_pointer_cast<std::vector<T>>(m_components[hash]);
	auto& indices = *m_componentIndices[hash];

	assert(!HasComponents<T>(_entity) && "Component already present in the _entity.");
	indices[_entity.m_id.m_index / 64u] |= (1ull << (_entity.m_id.m_index % 64u));
	components[_entity.m_id.m_index] = T(_args...);
}

template<typename T>
void ComponentManager::RemoveComponent(const Entity _entity)
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	const uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& indices = *m_componentIndices[hash];
	assert(HasComponents<T>(_entity) && "Tried to remove non-existing component.");

	indices[_entity.m_id.m_index / 64u] &= ~(1ull << (_entity.m_id.m_index % 64u));
}

template<typename T>
T& ComponentManager::GetComponent(const Entity _entity)
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	const uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& components = *std::static_pointer_cast<std::vector<T>>(m_components[hash]);
	assert(HasComponents<T>(_entity) && "Tried to access non-existing component.");

	return components[_entity.m_id.m_index];
}


// HAS ALL
template<typename T>
bool ComponentManager::HasComponents(const Entity _entity) const
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& indices = *m_componentIndices.at(hash);
	return (indices[_entity.m_id.m_index / 64u] & (1ull << (_entity.m_id.m_index % 64u))) != 0u;
}

template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasComponents(const Entity _entity) const
{
	return HasComponents<T1>(_entity) && HasComponents<T2, Args...>(_entity);
}

template<typename T>
bool ComponentManager::HasComponents(const EntityId _entityId) const
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& indices = *m_componentIndices.at(hash);
	return (indices[_entityId.m_index / 64u] & (1ull << (_entityId.m_index % 64u))) != 0u;
}

template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasComponents(const EntityId _entityId) const
{
	return HasComponents<T1>(_entityId) && HasComponents<T2, Args...>(_entityId);
}

template<typename T>
bool ComponentManager::HasComponents(const uint16 entityIndex) const
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& indices = *m_componentIndices.at(hash);
	return (indices[entityIndex / 64u] & (1ull << (entityIndex % 64u))) != 0u;
}

template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasComponents(const uint16 entityIndex) const
{
	return HasComponents<T1>(entityIndex) && HasComponents<T2, Args...>(entityIndex);
}

// HAS ANY
template<typename T>
bool ComponentManager::HasAnyComponents(const Entity _entity) const
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& indices = *m_componentIndices.at(hash);
	return (indices[_entity.m_id.m_index / 64u] & (1ull << (_entity.m_id.m_index % 64u))) != 0u;
}

template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasAnyComponents(const Entity _entity) const
{
	return HasAnyComponents<T1>(_entity) || HasAnyComponents<T2, Args...>(_entity);
}

template<typename T>
bool ComponentManager::HasAnyComponents(const EntityId _entityId) const
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& indices = *m_componentIndices.at(hash);
	return (indices[_entityId.m_index / 64u] & (1ull << (_entityId.m_index % 64u))) != 0u;
}

template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasAnyComponents(const EntityId _entityId) const
{
	return HasAnyComponents<T1>(_entityId) || HasAnyComponents<T2, Args...>(_entityId);
}

template<typename T>
bool ComponentManager::HasAnyComponents(const uint16 entityIndex) const
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& indices = *m_componentIndices.at(hash);
	return (indices[entityIndex / 64u] & (1ull << (entityIndex % 64u))) != 0u;
}

template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasAnyComponents(const uint16 entityIndex) const
{
	return HasAnyComponents<T1>(entityIndex) || HasAnyComponents<T2, Args...>(entityIndex);
}

// HAS NOT
template<typename T>
bool ComponentManager::HasNotComponents(const Entity _entity) const
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& indices = *m_componentIndices.at(hash);
	return (indices[_entity.m_id.m_index / 64u] & (1ull << (_entity.m_id.m_index % 64u))) == 0u;
}

template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasNotComponents(const Entity _entity) const
{
	return HasNotComponents<T1>(_entity) && HasNotComponents<T2, Args...>(_entity);
}

template<typename T>
bool ComponentManager::HasNotComponents(const EntityId _entityId) const
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& indices = *m_componentIndices.at(hash);
	return (indices[_entityId.m_index / 64u] & (1ull << (_entityId.m_index % 64u))) == 0u;
}

template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasNotComponents(const EntityId _entityId) const
{
	return HasNotComponents<T1>(_entityId) && HasNotComponents<T2, Args...>(_entityId);
}

template<typename T>
bool ComponentManager::HasNotComponents(const uint16 entityIndex) const
{
	assert(IsComponentRegistered<T>() && "Component has not been registered!");
	uint32 hash = Hash(typeid(T).name(), std::strlen(typeid(T).name()));

	auto& indices = *m_componentIndices.at(hash);
	return (indices[entityIndex / 64u] & (1ull << (entityIndex % 64u))) == 0u;
}

template<typename T1, typename T2, typename... Args>
bool ComponentManager::HasNotComponents(const uint16 entityIndex) const
{
	return HasNotComponents<T1>(entityIndex) && HasNotComponents<T2, Args...>(entityIndex);
}


ECS_NAMESPACE_END