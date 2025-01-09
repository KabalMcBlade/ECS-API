// Copyright (c) 2022-2025 Michele Condo'
// File: C:\Projects\ECS-API\ECS\component_manager.cpp
// Licensed under the MIT License. See LICENSE file in the project root for full license information.

#include "component_manager.h"

ECS_NAMESPACE_BEGIN

void ComponentManager::Create(uint16 _maxEntities, uint16 _maxComponents)
{
	assert(_maxEntities <= kMaxEntities && "Max Entities exceeds the maximum number of entities!");

	m_maxEntities = _maxEntities;
	m_componentHashId.reserve(_maxComponents);
}

void ComponentManager::Destroy()
{
	m_componentHashId.clear();
	m_components.clear();
	m_componentIndices.clear();
}


ECS_API ComponentManager& GetComponentManager()
{
	return ComponentManager::Instance();
}

ECS_NAMESPACE_END
