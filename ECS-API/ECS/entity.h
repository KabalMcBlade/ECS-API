#pragma once

#include "types.h"

#include <limits>
#include <utility>

ECS_NAMESPACE_BEGIN

static constexpr uint32 kMaxEntities = kMaxEntityIndex;

class Entity
{
public:
    Entity() = delete;
    
    bool operator==(const Entity _other) const;
    bool operator!=(const Entity _other) const;
    
    bool IsValid() const;
    
    uint32 GetIndex() const;
    uint32 GetVersion() const;

private:
    // For the CollectEntitiesWithAll only!
    Entity(uint16 _index, uint8 _version);
    
    EntityId m_id;
    
    friend class EntityManager;
    friend class ComponentManager;
    
    template<typename T, typename... Args>
    friend class IterateEntitiesWithAll;
    
    template<typename T, typename... Args>
    friend class IterateEntitiesWithAny;
    
    template<typename T, typename... Args>
    friend class IterateEntitiesWithNot;
};

ECS_NAMESPACE_END