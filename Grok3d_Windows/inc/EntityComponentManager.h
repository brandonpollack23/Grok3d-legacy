/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __WORLD__H

#define __WORLD__H

#include "grok3d.h"
#include "grok3d_types.h"

#include "EntityComponentManager.h"
#include "Entity/EntityHandle.h"

#include "Component/Component.h"
#include "Component/ComponentHandle.h"

#include "System/SystemManager.h"

#include <vector>
#include <unordered_map>

namespace Grok3d 
{
    class GRK_EntityComponentManager
    {
    public:
        GRK_EntityComponentManager();

        GRK_Result Initialize(Grok3d::Systems::GRK_SystemManager* systemManager);

        Grok3d::Entities::GRK_EntityHandle CreateEntity();

        //these component functions can only be called from GRK_EntityHandle or GRK_World!!!
        template<class ComponentType>
        Grok3d::GRK_Result AddComponent(Grok3d::Entities::GRK_Entity entity, ComponentType& newComponent);

        template<class ComponentType>
        Grok3d::Components::GRK_ComponentHandle<ComponentType> GetComponent(Grok3d::Entities::GRK_Entity entity)
        {
            if (
                entity == 0 ||
                (m_entityComponentsBitMaskMap[entity] & (IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>())) == 0)
                )
            {
                return Grok3d::Components::GRK_ComponentHandle<ComponentType>(nullptr, nullptr, -1);
            }
            else if ((m_entityComponentsBitMaskMap[entity] & IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>())) == 0)
            {
                //this is a vector of the type we are trying to remove
                std::vector<Components::GRK_Component>* componentTypeVector = &m_componentsStore[GRK_Component::GetComponentTypeAccessIndex<ComponentType>()];
                std::unordered_map<Entities::GRK_Entity, ComponentInstance>* entityInstanceMap = &m_entityComponentIndexMaps[GRK_Component::GetComponentTypeAccessIndex<ComponentType>()];

                ComponentInstance instance = (*entityInstanceMap)[entity];

                ComponentType* componentPointer = static_cast<ComponentType*>(&(*componentTypeVector)[instance]);

                return GRK_ComponentHandle<ComponentType>(this, componentPointer, entity);
            }
            else
            {
                return GRK_ComponentHandle<ComponentType>(nullptr, nullptr, -1);
            }

        }

        template<class ComponentType>
        Grok3d::GRK_Result RemoveComponent(Grok3d::Entities::GRK_Entity entity);

        Grok3d::Components::GRK_ComponentBitMask GetEntityComponentsBitMask(Grok3d::Entities::GRK_Entity entity);

        Grok3d::GRK_Result DeleteEntity(Grok3d::Entities::GRK_Entity entity);

        std::vector<Grok3d::Entities::GRK_Entity>& GetDeletedUncleanedEntities();

    private:
        bool m_isInitialized = false;

        Grok3d::Systems::GRK_SystemManager* m_systemManager;
        //entity index information
        Grok3d::Entities::GRK_Entity m_NextEntityId = 1;

        //list of deleted entites that need to be GC'd
        std::vector<Grok3d::Entities::GRK_Entity> m_deletedUncleatedEntities;

        //index into vector for component
        typedef int ComponentInstance;

        //this is a map of entities to a bitmask of their components, used for system registration/component deletion checks etc
        std::unordered_map<Grok3d::Entities::GRK_Entity, Grok3d::Components::GRK_ComponentBitMask> m_entityComponentsBitMaskMap;

        //vector of maps from entity to component index into componentStore[ComponentType::Offset]
        std::vector<std::unordered_map<Grok3d::Entities::GRK_Entity, ComponentInstance>> m_entityComponentIndexMaps;

        //vector of vectors of components, each index is the associated componenttype's list of components, indexed by m_EntityMaps vector
        std::vector<std::vector<Grok3d::Components::GRK_Component>> m_componentsStore;
    };
} /*Grok3d*/

#endif