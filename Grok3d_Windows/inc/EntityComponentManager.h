/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __WORLD__H

#define __WORLD__H

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
        // Stretch 2: Make CompomentManager::AddComponenet only take in entity, have it construct the component as part of the array and 
        //   make the arguments of that constructor also a generic Args template and pass that in here

        // adds a component of generic type to this manager and associates it with an entity
        // componenet parameter is by reference so it isn't copied twice when it is put in the array
        // I chose to have the component constructed and then copied because it is easier than passing a factory function
        // to this generic class and calling it on AddComponent with all it's parameters wrapped in a generic args class
        template<class ComponentType>
        Grok3d::GRK_Result AddComponent(Grok3d::Entities::GRK_Entity entity, ComponentType& newComponent)
        {
            if (entity == 0)
            {
                return GRK_Result::EntityAlreadyDeleted;
            }

            //only add a component if one doesnt exist
            if ((m_entityComponentsBitMaskMap[entity] & IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>())) == 0)
            {
                auto componentTypeIndex = GRK_Component::GetComponentTypeAccessIndex<ComponentType>();
                //this is a vector of the type we are trying to add
                // std::vector<GRK_Component>* componentTypeVector = nullptr;
                // if (m_componentsStore.size() == componentTypeIndex + 1)
                // {
                //     componentTypeVector = &(m_componentsStore[componentTypeIndex]);
                // }
                // else if (m_componentsStore.size() == componentTypeIndex)
                // {
                //     m_entityComponentIndexMaps.push_back(std::unordered_map<GRK_Entity, ComponentInstance>(INITIAL_ENTITY_ARRAY_SIZE));
                //     m_componentsStore.push_back(std::vector<GRK_Component>(INITIAL_ENTITY_ARRAY_SIZE));
                //     componentTypeVector = &m_componentsStore.back();
                // }
                // else
                // {
                //     return Grok3d::GRK_Result::CriticalError;
                // }
                std::vector<GRK_Component> componentTypeVector = GetComponentStore<ComponentType>();

                if (componentTypeVector.size() == componentTypeVector.max_size())
                {
                    return Grok3d::GRK_Result::NoSpaceRemaining;
                }
                else
                {
                    //TODO 8 have 2 CVARs that customizes load reallocation size, for now i'm doing 10% larger
                    //SCALE NUM and SCALE DEN
                    //DOCUMENTATION put the formula for scale factor of this in the docs, %scale = (1+NUM/DEN)

                    //resize vector if necessary
                    auto cap = componentTypeVector.capacity();
                    if (cap == componentTypeVector.size())
                    {
                        componentTypeVector.reserve(cap + cap / 10);
                    }

                    //add the component to the end our vector
                    componentTypeVector.push_back(newComponent);

                    //the new size - 1 is the index of the vector the element is stored at
                    std::unordered_map<GRK_Entity, ComponentInstance>* entityInstanceMap =
                        &m_entityComponentIndexMaps[componentTypeIndex];
                    (*entityInstanceMap)[entity] = static_cast<ComponentInstance>(componentTypeVector.size() - 1);

                    m_entityComponentsBitMaskMap[entity] |=
                        IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>());

                    //inform all systems of new component added to this entity
                    m_systemManager->UpdateSystemEntities(GRK_EntityHandle(this, entity));

                    return GRK_Result::Ok;
                }
            }
            else
            {
                return GRK_Result::ComponentAlreadyAdded;
            }
        }

        template<class ComponentType>
        Grok3d::Components::GRK_ComponentHandle<ComponentType> GetComponent(Grok3d::Entities::GRK_Entity entity)
        {
            GRK_ComponentBitMask componentMask =
                static_cast<GRK_ComponentBitMask>(IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>()));

            if (entity == 0 || (m_entityComponentsBitMaskMap.at(entity) & componentMask) == 0)
            {
                return Grok3d::Components::GRK_ComponentHandle<ComponentType>(nullptr, nullptr, -1);
            }
            else if ((m_entityComponentsBitMaskMap.at(entity) & componentMask) == componentMask)
            {
                //this is a vector of the type we are trying to remove
                const std::vector<ComponentType> componentTypeVector = GetComponentStore();
                const std::unordered_map<Entities::GRK_Entity, ComponentInstance>* entityInstanceMap =
                    &m_entityComponentIndexMaps.at(GRK_Component::GetComponentTypeAccessIndex<ComponentType>());

                //get the instance (index in our vector) from teh entityInstanceMap
                ComponentInstance instance = (*entityInstanceMap).at(entity);

                //use the instance to index the array of that componenttype
                ComponentType* componentPointer = &(componentTypeVector.at(instance));

                //return it in a handle
                return Grok3d::Components::GRK_ComponentHandle<ComponentType>(this, componentPointer, entity);
            }
            else
            {
                return Grok3d::Components::GRK_ComponentHandle<ComponentType>(nullptr, nullptr, -1);
            }

        }

        //removes component for specified entity if it exists
        //TODO Garbage collect this instead of deleting immediately by maintaining a list
        template<class ComponentType>
        Grok3d::GRK_Result RemoveComponent(Grok3d::Entities::GRK_Entity entity)
        {
            GRK_ComponentBitMask componentMask = static_cast<GRK_ComponentBitMask>(IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>()));

            if (entity == 0)
            {
                return GRK_Result::EntityAlreadyDeleted;
            }

            if ((m_entityComponentsBitMaskMap[entity] & componentMask) > 0)
            {
                RemoveComponentHelper(entity, ComponentType::GetComponentTypeAccessIndex());
            }
            else
            {
                return GRK_Result::NoSuchElement;
            }
        }

        Grok3d::Components::GRK_ComponentBitMask GetEntityComponentsBitMask(Grok3d::Entities::GRK_Entity entity);

        Grok3d::GRK_Result DeleteEntity(Grok3d::Entities::GRK_Entity entity);

        void GarbageCollect();

        std::vector<Grok3d::Entities::GRK_Entity>& GetDeletedUncleanedEntities();

    private:
        Grok3d::GRK_Result RemoveComponentHelper(Grok3d::Entities::GRK_Entity entity, size_t componentAccessIndex);

        template<class ComponentType>
        std::vector<ComponentType>& GetComponentStore()
        {
            static std::vector<ComponentType> store = InitializeComponentStore();
            return store;
        }

        template<class ComponentType>
        std::vector<ComponentType>& InitializeComponentStore()
        {
            static std::vector<ComponentType> store;
            store.reserve(INITIAL_ENTITY_ARRAY_SIZE);
            return store;
        }

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
    };
} /*Grok3d*/

#endif
