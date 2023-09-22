#include "entity.h"

// DEBUG ONLY
#include <iostream>
//#include <type_traits>

ECS_NAMESPACE_BEGIN

Entity::Entity(uint16 _index, uint8 _version) : m_id({_index, _version}) { }

bool Entity::operator==(const Entity _other) const
{
	return m_id.m_index == _other.m_id.m_index && m_id.m_version == _other.m_id.m_version;
}

bool Entity::operator!=(const Entity _other) const
{
	return !((*this) == _other);
}

bool Entity::IsValid() const
{
    return m_id.m_index >= 0u && m_id.m_version > 0u;
}

uint32 Entity::GetIndex() const
{
	return static_cast<uint32>(m_id.m_index);
}

uint32 Entity::GetVersion() const
{
	return static_cast<uint32>(m_id.m_version);
}

ECS_NAMESPACE_END