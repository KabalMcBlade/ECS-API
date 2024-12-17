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


ECS_DLL ComponentManager& GetComponentManager()
{
	return ComponentManager::Instance();
}

ECS_NAMESPACE_END
