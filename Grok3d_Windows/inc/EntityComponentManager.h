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
        // Stretch 2: Make CompomentManager::AddComponenet only take in entity, have it construct the component as part of the array and 
        //   make the arguments of that constructor also a generic Args template and pass that in here

        //adds a component of generic type to this manager and associates it with an entity
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
                //this is a vector of the type we are trying to add
                std::vector<GRK_Component>* componentTypeVector = &(m_componentsStore[GRK_Component::GetComponentTypeAccessIndex<ComponentType>()]);

                if (componentTypeVector->size() == componentTypeVector->max_size())
                {
                    return GRK_Result::NoSpaceRemaining;
                }
                else
                {
                    //TODO 8 have 2 CVARs that customizes load reallocation size, for now i'm doing 10% larger
                    //SCALE NUM and SCALE DEN
                    //DOCUMENTATION put the formula for scale factor of this in the docs, %scale = (1+NUM/DEN)
                    //resize vector if necessary
                    auto cap = componentTypeVector->capacity();
                    if (cap == componentTypeVector->size())
                    {
                        componentTypeVector->reserve(cap + cap / 10);
                    }

                    //add the component to the end our vector
                    componentTypeVector->push_back(newComponent);

                    //the new size - 1 is the index of the vector the element is stored at
                    m_entityComponentIndexMaps[GRK_Component::GetComponentTypeAccessIndex<ComponentType>()][entity] = static_cast<int>(componentTypeVector->size() - 1);

                    m_entityComponentsBitMaskMap[entity] |= IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>());

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

        //removes component for specified entity if it exists
        //TODO Garbage collect this instead of deleting immediately by maintaining a list
        template<class ComponentType>
        Grok3d::GRK_Result RemoveComponent(Grok3d::Entities::GRK_Entity entity)
        {
            if (entity == 0)
            {
                return GRK_Result::EntityAlreadyDeleted;
            }

            if (m_entityComponentsBitMaskMap[entity] & IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>()) != 0)
            {
                //this is a vector of the type we are trying to remove
                std::vector<GRK_Component*>* componentTypeVector = &m_componentsStore[ComponentType::GetComponentTypeAccessIndex()];
                //this is the map of entity to components for this type
                std::unordered_map<GRK_Entity, GRK_Component*>* entityInstanceMap = &m_entityComponentIndexMaps[GRK_Component::GetComponentTypeAccessIndex<ComponentType>()];

                //check if the elment is in the map
                //and do what we need to if it is not
                auto it = *entityInstanceMap.find(entity);
                if (it == *entityInstanceMap.end())
                {
                    return GRK_NOSUCHELEMENT;
                }
                else
                {
                    //this entity exists so we move the last element of the components vector
                    //to the spot that this one was taking up
                    auto indexToMoveLastStoredComponent = *entityInstanceMap[entity];
                    auto lastElement = componentTypeVector->back();
                    //use std::move so we cannibilize any allocated components and dont copy them
                    //TODO 3 implement std::move constructor (&&) for GRK_Components
                    *(componentTypeVector)[indexToMoveLastStoredComponent] = std::move(lastElement);

                    //then remove it from the map
                    //and shorten our vector
                    entityInstanceMap->erase(it);
                    componentTypeVector->pop_back();

                    //remove it from bitmask
                    *(m_entityComponentsBitMaskMap)[entity] &= ~(IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>()));

                    return GRK_Result::Ok;
                }
            }
            else
            {
                return GRK_Result::NoSuchElement;
            }
        }

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