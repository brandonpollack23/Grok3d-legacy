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

#include <stdexcept>

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::Entities;
using namespace Grok3d::Systems;

GRK_Entity GRK_EntityComponentManager::s_NextEntityId = 1;

GRK_EntityComponentManager::GRK_EntityComponentManager() :
    m_entityComponentsBitMaskMap(std::unordered_map<GRK_Entity, GRK_ComponentBitMask>(INITIAL_ENTITY_ARRAY_SIZE)),
    m_deletedUncleanedEntities(std::vector<GRK_Entity>(INITIAL_ENTITY_ARRAY_SIZE/4)),
    m_entityComponentIndexMaps(std::vector<std::unordered_map<GRK_Entity, std::tuple<ComponentInstance, GRK_Entity>>>(1))
{
    m_entityComponentIndexMaps[0] = std::unordered_map<GRK_Entity, std::tuple<ComponentInstance, GRK_Entity>>(INITIAL_ENTITY_ARRAY_SIZE);
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
    GRK_Entity id = s_NextEntityId++;

    m_entityComponentsBitMaskMap[id] = 0;

    this->AddComponent(id, GRK_TransformComponent());

    return GRK_EntityHandle(this, id);
}

GRK_ComponentBitMask GRK_EntityComponentManager::GetEntityComponentsBitMask(const GRK_Entity entity) const
{
    if (entity != 0)
    {
        return m_entityComponentsBitMaskMap.at(entity);
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
    m_deletedUncleanedEntities.push_back(entity);

    // send a component bit mask of 0 to all Systems, no components means any system will unregister
    m_systemManager->UnregisterEntity(GRK_EntityHandle(this, entity));

    return result;
}

void GRK_EntityComponentManager::GarbageCollect()
{
    //TODO dont always do this lets be smarter
    for (const auto& entity : m_deletedUncleanedEntities)
    {
        GRK_ComponentBitMask componentBitMask = m_entityComponentsBitMaskMap[entity];
        for (int i = 0; i < GRK_Component::NumberOfComponentTypes(); i++)
        {
            GRK_ComponentBitMask componentMask = componentBitMask & (1 << i);

            if((m_entityComponentsBitMaskMap[entity] & componentMask) > 0)
            {
                //I know this line looks complex, but it's just bad c++ syntax
                //member function pointers are not as simple in the type system as function pointers
                //This is because they need "this" which is the implicit first parameter to the function
                //so you deref this, pass the function pointer (also stored in a vector that is a member of this, ironically) 
                //wrap that in parens because that is our dereferenced this->func, and then pass 
                //that function's params in another set of parens
                (this->*m_removeComponentHelperMap[i])(entity);
            }
        }

        m_entityComponentsBitMaskMap.erase(entity);
    }

    m_deletedUncleanedEntities.clear();
}

std::vector<GRK_Entity>& GRK_EntityComponentManager::GetDeletedUncleanedEntities()
{
    return m_deletedUncleanedEntities;
}

