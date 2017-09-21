/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "EntityComponentManager.h"

#include "Entity/EntityHandle.h"

#include "Component/Component.h"
#include "Component/ComponentHandle.h"
#include "Component/TransformComponent.h"

#include "System/SystemManager.h"

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::Entities;
using namespace Grok3d::Systems;

GRK_EntityComponentManager::GRK_EntityComponentManager() :
    m_entityComponentsBitMaskMap(std::unordered_map<GRK_Entity, GRK_ComponentBitMask>(INITIAL_ENTITY_ARRAY_SIZE)),
    m_deletedUncleatedEntities(std::vector<GRK_Entity>(INITIAL_ENTITY_ARRAY_SIZE/4)),
    m_entityComponentIndexMaps(std::vector<std::unordered_map<GRK_Entity, ComponentInstance>>(1)),
    m_componentsStore(std::vector<std::vector<GRK_Component>>(1))
{
    m_entityComponentIndexMaps[0] = std::unordered_map<GRK_Entity, ComponentInstance>(INITIAL_ENTITY_ARRAY_SIZE);
    m_componentsStore[0] = std::vector<GRK_Component>();
    m_componentsStore[0].reserve(INITIAL_ENTITY_ARRAY_SIZE);
}

GRK_Result GRK_EntityComponentManager::Initialize(GRK_SystemManager* systemManager)
{
    m_systemManager = systemManager;
    m_isInitialized = true;

    return GRK_Result::Ok;
}

GRK_EntityHandle GRK_EntityComponentManager::CreateEntity()
{
    //I could do a check here to see if we overflowed to 0 but that's just inconceivable that we'd have that many (2^32) entities    
    GRK_Entity id = m_NextEntityId++;

    m_entityComponentsBitMaskMap[id] = 0;

    this->AddComponent(id, GRK_TransformComponent());

    return GRK_EntityHandle(this, id);
}

GRK_ComponentBitMask GRK_EntityComponentManager::GetEntityComponentsBitMask(GRK_Entity entity)
{
    if (entity != 0)
    {
        return m_entityComponentsBitMaskMap[entity];
    }
    else 
    {
        return 0;
    }
}

GRK_Result GRK_EntityComponentManager::DeleteEntity(GRK_Entity entity)
{
    GRK_Result result = GRK_Result::Ok;

    //ComponentManager's deleting their components is handled by GarbageCollection
    m_deletedUncleatedEntities.push_back(entity);

    // send a component bit mask of 0 to all Systems, no components means any system will unregister
    m_systemManager->UnregisterEntity(GRK_EntityHandle(this, entity));

    return result;
}

void GRK_EntityComponentManager::GarbageCollect()
{
    //TODO dont always do this lets be smarter
    for (const auto& entity : m_deletedUncleatedEntities)
    {
        GRK_ComponentBitMask componentBitMask = m_entityComponentsBitMaskMap[entity];
        for (int i = 0; i < GRK_Component::NumberOfComponentTypes(); i++)
        {
            GRK_ComponentBitMask componentMask = componentBitMask & (1 << i);

            if((m_entityComponentsBitMaskMap[entity] & componentMask) > 0)
            {
                this->RemoveComponentHelper(componentMask, entity);
            }
        }

        m_entityComponentsBitMaskMap.erase(entity);
    }

    m_deletedUncleatedEntities.clear();
}

std::vector<GRK_Entity>& GRK_EntityComponentManager::GetDeletedUncleanedEntities()
{
    return m_deletedUncleatedEntities;
}

GRK_Result GRK_EntityComponentManager::RemoveComponentHelper(GRK_Entity entity, size_t componentAccessIndex)
{
    //this is a vector of the type we are trying to remove
    std::vector<GRK_Component>& componentTypeVector = m_componentsStore[componentAccessIndex];

    //this is the map of entity to components for this type
    std::unordered_map<GRK_Entity, ComponentInstance> entityInstanceMap = m_entityComponentIndexMaps[componentAccessIndex];

    //check if the elment is in the map
    //and do what we need to if it is not
    auto it = entityInstanceMap.find(entity);
    if (it == entityInstanceMap.end())
    {
        return GRK_Result::NoSuchElement;
    }
    else
    {
        //this entity exists so we move the last element of the components vector
        //to the spot that this one was taking up
        auto indexToMoveLastStoredComponent = entityInstanceMap[entity];
        auto lastElement = componentTypeVector.back();
        //use std::move so we cannibilize any allocated components and dont copy them
        componentTypeVector[indexToMoveLastStoredComponent] = std::move(lastElement);

        //then remove it from the map
        //and shorten our vector
        entityInstanceMap.erase(it);
        componentTypeVector.pop_back();

        //remove it from bitmask
        m_entityComponentsBitMaskMap[entity] &= ~(IndexToMask(componentAccessIndex));

        return GRK_Result::Ok;
    }
}