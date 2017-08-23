/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "EntityComponentManager.h"

#include "Entity/EntityHandle.h"

#include "Component/Component.h"
#include "Component/ComponentHandle.h"

#include "System/SystemManager.h"

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::Entities;
using namespace Grok3d::Systems;

GRK_EntityComponentManager::GRK_EntityComponentManager()
{
    //TODO 10 INITIAL_ENTITY_ARRAY_SIZE is a init CVAR just like in ComponentManager
    m_entityComponentsBitMaskMap = std::unordered_map<GRK_Entity, GRK_ComponentBitMask>(INITIAL_ENTITY_ARRAY_SIZE);
    m_deletedUncleatedEntities = std::vector<GRK_Entity>(INITIAL_ENTITY_ARRAY_SIZE/4);

    //intialize entityInstanceMaps to 1 since the only REQUIRED componenttype is TransformComponent
    m_entityComponentIndexMaps = std::vector<std::unordered_map<GRK_Entity, ComponentInstance>>(1);
    m_entityComponentIndexMaps[0] = std::unordered_map<GRK_Entity, ComponentInstance>(INITIAL_ENTITY_ARRAY_SIZE);

    //same goes for componentStore
    m_componentsStore = std::vector<std::vector<GRK_Component>>(1);
    m_componentsStore[0] = std::vector<GRK_Component>(INITIAL_ENTITY_ARRAY_SIZE);
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
    m_entityComponentsBitMaskMap.erase(entity);
    m_deletedUncleatedEntities.push_back(entity);

    // send a component bit mask of 0 to all Systems, no components means any system will unregister
    m_systemManager->UnregisterEntity(GRK_EntityHandle(this, entity));

    return result;
}

std::vector<GRK_Entity>& GRK_EntityComponentManager::GetDeletedUncleanedEntities()
{
    return m_deletedUncleatedEntities;
}