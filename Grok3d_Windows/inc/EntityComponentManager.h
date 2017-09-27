/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __ENTITYCOMPONENENTMANAGER__H
#define __ENTITYCOMPONENENTMANAGER__H

#include "grok3d_types.h"

#include "Entity/EntityHandle.h"

#include "Component/Component.h"
#include "Component/ComponentHandle.h"

#include "System/SystemManager.h"

#include <vector>
#include <unordered_map>
#include <functional>

namespace Grok3d
{
    class GRK_EntityComponentManager
    {
    public:
        GRK_EntityComponentManager();

        GRK_Result Initialize(Grok3d::Systems::GRK_SystemManager* systemManager);

        Grok3d::Entities::GRK_EntityHandle CreateEntity();

        //these component functions can only be called from GRK_EntityHandle or GRK_World!!!
        // Stretch 2: Make CompomentManager::AddComponent only take in entity, have it construct the component as part of the array and 
        //   make the arguments of that constructor also a generic Args template and pass that in here

        // adds a component of generic type to this manager and associates it with an entity
        // Component parameter is by reference so it isn't copied twice when it is put in the array
        // I chose to have the component constructed and then copied because it is easier than passing a factory function
        // to this generic class and calling it on AddComponent with all it's parameters wrapped in a generic args class
        template<class ComponentType>
        Grok3d::GRK_Result AddComponent(Grok3d::Entities::GRK_Entity entity, ComponentType& newComponent)
        {
            static_assert(std::is_base_of<GRK_Component, ComponentType>::value, "AddComponent Function requires template parameter based on GRK_Component");

            if (entity == 0)
            {
                return GRK_Result::EntityAlreadyDeleted;
            }

            auto componentTypeIndex = GRK_Component::GetComponentTypeAccessIndex<ComponentType>();

            //only add a component if one doesnt exist
            if ((m_entityComponentsBitMaskMap[entity] & IndexToMask(componentTypeIndex)) == 0)
            {
                std::vector<ComponentType>& componentTypeVector = GetComponentStore<ComponentType>();

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
                        IndexToMask(componentTypeIndex);

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

        //TODO make const, what is with the initializer list error? is it a bug?
        template<class ComponentType>
        Grok3d::Components::GRK_ComponentHandle<ComponentType> GetComponent(Grok3d::Entities::GRK_Entity entity) const
        {
            static_assert(std::is_base_of<GRK_Component, ComponentType>::value, "GetComponent Function requires template parameter based on GRK_Component");

            GRK_ComponentBitMask componentMask =
                static_cast<GRK_ComponentBitMask>(IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>()));

            if (entity == 0 || (m_entityComponentsBitMaskMap.at(entity) & componentMask) == 0)
            {
                return Grok3d::Components::GRK_ComponentHandle<ComponentType>(nullptr, nullptr, -1);
            }
            else if ((m_entityComponentsBitMaskMap.at(entity) & componentMask) == componentMask)
            {
                //this is a vector of the type we are trying to remove
                std::vector<ComponentType>& componentTypeVector = GetComponentStore<ComponentType>();
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
        template<class ComponentType>
        Grok3d::GRK_Result RemoveComponent(Grok3d::Entities::GRK_Entity entity)
        {
            static_assert(std::is_base_of<GRK_Component, ComponentType>::value, "RemoveComponent Function requires template parameter based on GRK_Component");

            GRK_ComponentBitMask componentMask = static_cast<GRK_ComponentBitMask>(IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>()));

            if (entity == 0)
            {
                return GRK_Result::EntityAlreadyDeleted;
            }

            if ((m_entityComponentsBitMaskMap[entity] & componentMask) > 0)
            {
                //TODO GC this as well, when you remove component you should just remove it from
                //entity mask and store the actual component from the store's ComponentInstance in a
                //m_deletedUncleanedComponents vector to iterate through
                //there may be some overlap with the entity removal pass so use .find(entity) and
                //check to be sure it is already out of the map
                RemoveComponentHelper(entity, ComponentType::GetComponentTypeAccessIndex());
            }
            else
            {
                return GRK_Result::NoSuchElement;
            }
        }

        Grok3d::Components::GRK_ComponentBitMask GetEntityComponentsBitMask(Grok3d::Entities::GRK_Entity entity) const;

        Grok3d::GRK_Result DeleteEntity(Grok3d::Entities::GRK_Entity entity);

        void GarbageCollect();

        std::vector<Grok3d::Entities::GRK_Entity>& GetDeletedUncleanedEntities();

    private:
        template<class ComponentType>
        Grok3d::GRK_Result GRK_EntityComponentManager::RemoveComponentHelper(Grok3d::Entities::GRK_Entity entity)
        {
            static_assert(std::is_base_of<GRK_Component, ComponentType>::value, "RemoveComponentHelper Function requires template parameter based on GRK_Component");

            auto componentAccessIndex = Grok3d::Components::GRK_Component::GetComponentTypeAccessIndex<ComponentType>();
            //this is a vector of the type we are trying to remove
            std::vector<ComponentType>& componentTypeVector = this->GetComponentStore<ComponentType>();

            //this is the map of entity to components for this type
            std::unordered_map<Grok3d::Entities::GRK_Entity, ComponentInstance> entityInstanceMap = m_entityComponentIndexMaps[componentAccessIndex];

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

        template<class ComponentType>
        std::vector<ComponentType>& GetComponentStore() const
        {
            static std::vector<ComponentType>& store = InitializeComponentStore<ComponentType>();
            return store;
        }

        template<class ComponentType>
        std::vector<ComponentType>& InitializeComponentStore() const
        {
            //create component store vector
            static std::vector<ComponentType> store;
            store.reserve(INITIAL_ENTITY_ARRAY_SIZE);

            //add entity to ComponentInstance index map for this component type
            m_entityComponentIndexMaps.push_back(std::unordered_map<Grok3d::Entities::GRK_Entity, ComponentInstance>(INITIAL_ENTITY_ARRAY_SIZE));

            //add function to removal map for GC
            m_removeComponentHelperMap.push_back(&GRK_EntityComponentManager::RemoveComponentHelper<ComponentType>);

            return store;
        }

    private:
        bool m_isInitialized = false;

        Grok3d::Systems::GRK_SystemManager* m_systemManager;
        //entity index information
        Grok3d::Entities::GRK_Entity m_NextEntityId = 1;

        //list of deleted entites that need to be GC'd
        std::vector<Grok3d::Entities::GRK_Entity> m_deletedUncleanedEntities;

        //index into vector for component
        typedef size_t ComponentInstance;

        //this is a map of entities to a bitmask of their components, used for system registration/component deletion checks etc
        std::unordered_map<Grok3d::Entities::GRK_Entity, Grok3d::Components::GRK_ComponentBitMask> m_entityComponentsBitMaskMap;

        //vector of maps from entity to component index into componentStore[ComponentType::Offset]
        mutable std::vector<std::unordered_map<Grok3d::Entities::GRK_Entity, ComponentInstance>> m_entityComponentIndexMaps;

        typedef Grok3d::GRK_Result (GRK_EntityComponentManager::*RemoveComponentMemberFunc)(Grok3d::Entities::GRK_Entity);
        mutable std::vector<RemoveComponentMemberFunc> m_removeComponentHelperMap;
    };
} /*Grok3d*/

#endif
