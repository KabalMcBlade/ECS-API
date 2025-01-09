// Copyright (c) 2022-2025 Michele Condo'
// File: C:\Projects\ECS-API\ECS\entity_manager.cpp
// Licensed under the MIT License. See LICENSE file in the project root for full license information.

#include "entity_manager.h"

ECS_NAMESPACE_BEGIN

void EntityManager::Create(uint16 _maxEntities)
{
	assert(_maxEntities <= kMaxEntities && "Max entities exceeded!");
	m_totalEntityCreated = 0;
	m_entitiesVersion.resize(_maxEntities, 1u);

	const uint32 capacity = GetRequiredAmountOfUint64ToStoreBits(_maxEntities);
	m_entities.resize(capacity, 0u);
}

void EntityManager::Destroy()
{
	m_entities.clear();
	m_entitiesVersion.clear();
}

Entity EntityManager::CreateEntity()
{
	uint16 index = 0;
	for (const uint64 block : m_entities)
	{
		const uint64 invertedBlock = ~block;
		if (invertedBlock != 0)
		{
			index += static_cast<uint16>(CountTrailingZeros64(invertedBlock));
			break;
		}

		index += 64u;
	}

	assert(index < kMaxEntities && "Cannot create more entity!");

	++m_totalEntityCreated;

	// set bit
	m_entities[index / 64u] |= (1ull << (index % 64u));

	return { index, m_entitiesVersion[index] };
}

void EntityManager::DestroyEntity(const Entity _entity)
{
	--m_totalEntityCreated;

	const uint32 index = _entity.GetIndex();

	uint8& version = m_entitiesVersion[index];
	version %= 255u;
	++version;

	// clear bit
	m_entities[index / 64u] &= ~(1ull << (index % 64u));
}

bool EntityManager::ExistEntity(const uint16 _entityIndex) const
{
	return (m_entities[_entityIndex / 64u] & (1ull << (_entityIndex % 64u))) != 0u;
}

Entity EntityManager::GetEntity(const uint16 _entityIndex) const
{
	assert(_entityIndex >= 0 && _entityIndex < kMaxEntities && "Entity index out of range!");
	assert(ExistEntity(_entityIndex) && "Entity index does not exist!");

	return { _entityIndex, m_entitiesVersion[_entityIndex] };
}


ECS_API EntityManager& GetEntityManager()
{
	return EntityManager::Instance();
}

ECS_NAMESPACE_END
