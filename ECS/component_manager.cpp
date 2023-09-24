#ifdef ECS_USE_PCH
#include "pch.h"
#endif
#include "component_manager.h"

ECS_NAMESPACE_BEGIN

std::vector<uint32> ComponentManager::m_componentHashId;
uint16 ComponentManager::m_maxEntities;

ECS_NAMESPACE_END
