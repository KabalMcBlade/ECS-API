#pragma once

#include "types.h"
#include "entity.h"
#include "utility.h"

#include <cassert>
#include <vector>
#include <algorithm>
#include <limits>


ECS_NAMESPACE_BEGIN

namespace EntityManager
{
	ECS_DLL extern std::vector<uint64> m_entities;
	ECS_DLL extern std::vector<uint8> m_entitiesVersion;
	ECS_DLL extern uint16 m_totalEntityCreated;

	////////////////////////////////////////////////

	inline void Create(uint16 _maxEntities)
	{
		assert(_maxEntities <= kMaxEntities && "_maxEntitis parameter exceeds the maximum number of entities which is 16777215!");

		m_totalEntityCreated = 0;

		m_entitiesVersion.resize(_maxEntities, 1u);

		const uint32 capacity = GetRequiredAmountOfUint64ToStoreBits(_maxEntities);
		m_entities.resize(capacity, 0u);
	}

	inline void Destroy()
	{
		m_entities.clear();
		m_entitiesVersion.clear();
	}

	inline Entity CreateEntity()
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

	inline void DestroyEntity(const Entity _entity)
	{
		--m_totalEntityCreated;

		const uint32 index = _entity.GetIndex();

		uint8& version = m_entitiesVersion[index];
		version %= 255u;
		++version;

		// clear bit
		m_entities[index / 64u] &= ~(1ull << (index % 64u));
	}

	inline bool ExistEntity(const uint16 _entityIndex)
	{
		return (m_entities[_entityIndex / 64u] & (1ull << (_entityIndex % 64u))) != 0u;
	}

	inline Entity GetEntity(const uint16 _entityIndex)
	{
		assert(_entityIndex >= 0 && _entityIndex < kMaxEntities && "Entity index out of range!");
		assert(ExistEntity(_entityIndex) && "Entity index does not exist!");

		return { _entityIndex, m_entitiesVersion[_entityIndex] };
	}
};

ECS_NAMESPACE_END
