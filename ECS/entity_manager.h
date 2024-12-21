#pragma once

#include "types.h"
#include "entity.h"
#include "utility.h"

#include <cassert>
#include <vector>
#include <algorithm>
#include <limits>

// for now, for the warning of exporting std classes
#pragma warning(disable : 4251)

ECS_NAMESPACE_BEGIN

class ECS_DLL EntityManager
{
public:
	inline static EntityManager& Instance()
	{
		static EntityManager instance;
		return instance;
	}

	void Create(uint16 _maxEntities);
	void Destroy();

	Entity CreateEntity();
	void DestroyEntity(const Entity _entity);
	bool ExistEntity(const uint16 _entityIndex) const;
	Entity GetEntity(const uint16 _entityIndex) const;

	ECS_FORCE_INLINE uint16 GetTotalEntityCreated() const { return m_totalEntityCreated; }

	ECS_FORCE_INLINE const std::vector<uint64>& GetEntities() const { return m_entities; }

private:
	EntityManager() = default;
	~EntityManager() = default;

	EntityManager(const EntityManager&) = delete;
	EntityManager& operator=(const EntityManager&) = delete;

	std::vector<uint64> m_entities;
	std::vector<uint8> m_entitiesVersion;
	uint16 m_totalEntityCreated = 0;
};

ECS_DLL EntityManager& GetEntityManager();

ECS_NAMESPACE_END
