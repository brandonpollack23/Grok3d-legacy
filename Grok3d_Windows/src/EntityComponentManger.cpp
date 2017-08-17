/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "EntityComponentManager.h"

#include "Entity/EntityHandle.h"

#include "Component/Component.h"
#include "Component/ComponentHandle.h"

#include "Assertions.h"

using namespace Grok3d;
using namespace Grok3d::Assertions;
using namespace Grok3d::Components;
using namespace Grok3d::Entities;

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

GRK_EntityHandle GRK_EntityComponentManager::CreateEntity()
{
    //I could do a check here to see if we overflowed to 0 but that's just inconceivable that we'd have that many (2^32) entities    
    GRK_Entity id = m_NextEntityId++;

    m_entityComponentsBitMaskMap[id] = 0;

    this->AddComponent(id, GRK_TransformComponent());

    return GRK_EntityHandle(this, id);
}

// Stretch 2: Make CompomentManager::AddComponenet only take in entity, have it construct the component as part of the array and 
//   make the arguments of that constructor also a generic Args template and pass that in here

//adds a component of generic type to this manager and associates it with an entity
// componenet parameter is by reference so it isn't copied twice when it is put in the array
// I chose to have the component constructed and then copied because it is easier than passing a factory function
// to this generic class and calling it on AddComponent with all it's parameters wrapped in a generic args class
template<class ComponentType>
GRK_Result GRK_EntityComponentManager::AddComponent(GRK_Entity entity, ComponentType& newComponent)
{
    if (entity == 0)
    {
        return GRK_Result::EntityAlreadyDeleted;
    }

    //only add a component if one doesnt exist
    if((m_entityComponentsBitMaskMap[entity] & IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>())) == 0)
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

//removes component for specified entity if it exists
//TODO Garbage collect this instead of deleting immediately by maintaining a list
template<class ComponentType>
GRK_Result GRK_EntityComponentManager::RemoveComponent(GRK_Entity entity)
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

template<class ComponentType>
GRK_ComponentHandle<ComponentType>* GRK_EntityComponentManager::GetComponent(GRK_Entity entity)
{
    if (
        entity == 0 ||
        (m_entityComponentsBitMaskMap[entity] & IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>()) == 0))
    {
        return nullptr;
    }

    if ((m_entityComponentsBitMaskMap[entity] & IndexToMask(GRK_Component::GetComponentTypeAccessIndex<ComponentType>())) == 0)
    {
        //this is a vector of the type we are trying to remove
        std::vector<GRK_Component*>* componentTypeVector = &m_componentsStore[ComponentType::GetComponentTypeAccessIndex()];
        std::unordered_map<GRK_Entity, ComponentInstance>* entityInstanceMap = m_entityComponentIndexMaps[GRK_Component::GetComponentTypeAccessIndex<ComponentType>()];

        ComponentInstance instance = entityInstanceMap[entity];

        ComponentType* componentPointer = static_cast<ComponentType*>(&(*componentTypeVector[instance]));

        return GRK_ComponentHandle<ComponentType>(this, componentPointer, entity);
    }
    else
    {
        return GRK_Result::NoSuchElement;
    }
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

    //TODO 7 iterate through all systems delete entity

    m_entityComponentsBitMaskMap.erase(entity);
    m_deletedUncleatedEntities.push_back(entity);

    return result;
}

//TODO 20 change to GC
std::vector<GRK_Entity>& GRK_EntityComponentManager::GetDeletedUncleanedEntities()
{
    return m_deletedUncleatedEntities;
}