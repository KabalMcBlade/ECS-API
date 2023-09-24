#ifdef ECS_USE_PCH
#include "pch.h"
#endif
#include "entity_manager.h"

ECS_NAMESPACE_BEGIN

std::vector<uint64> EntityManager::m_entities;
std::vector<uint8> EntityManager::m_entitiesVersion;
uint16 EntityManager::m_totalEntityCreated;

ECS_NAMESPACE_END
