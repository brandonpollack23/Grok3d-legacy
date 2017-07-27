/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

#include "World.h"
#include "Component/ComponentManager.h"

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::Entities;

//TODO 8 that determines initial entity allocation size, here it is INITIAL_ENTITY_ARRAY_SIZE
template<class ComponentType>
GRK_ComponentManager<ComponentType>::GRK_ComponentManager(GRK_World* world) : m_world(world)
{
    m_entityMap = std::unordered_map<ComponentType>(INITIAL_ENTITY_ARRAY_SIZE);
    m_components = std::vector<ComponentType>(INITIAL_ENTITY_ARRAY_SIZE);
}

template<class ComponentType>
GRK_Result GRK_ComponentManager<ComponentType>::DeleteEntity(GRK_Entity entity)
{
    GRK_Result result = RemoveComponent(entity);

    if(result == GRK_Result::Ok)
    {
        m_entityMap.erase(entity);
    }

    return result;
}

// Stretch 2: Make CompomentManager::AddComponenet only take in entity, have it construct the component as part of the array and 
//   make the arguments of that constructor also a generic Args template and pass that in here

//adds a component of generic type to this manager and associates it with an entity
// componenet parameter is by reference so it isn't copied twice when it is put in the array
// I chose to have the component constructed and then copied because it is easier than passing a factory function
// to this generic class and calling it on AddComponent with all it's parameters wrapped in a generic args class
template<class ComponentType>
GRK_Result GRK_ComponentManager<ComponentType>::AddComponent(GRK_Entity entity, ComponentType& component)
{
    if (m_components.size() == m_components.max_size())
    {
        return GRK_NOSPACEREMAINING;
    }
    else
    {
        //TODO 8 have 2 CVARs that customizes load reallocation size, for now i'm doing 10% larger
        //SCALE NUM and SCALE DEN
        //DOCUMENTATION put the formula for scale factor of this in the docs, %scale = (1+NUM/DEN)
        //resize vector if necessary
        auto cap = m_components.capacity();
        if (cap == m_components.size())
        {
            m_components.reserve(cap + cap / 10);
        }

        //add the component to the end our vector
        m_components.push_back(component);

        //the new size - 1 is the index of the vector the element is stored at
        m_entityMap[entity] = m_components.size() - 1;

        return GRK_OK;
    }
}

//gets component of this generic type associated with given entity ID
template<class ComponentType>
ComponentType* GRK_ComponentManager<ComponentType>::GetComponent(GRK_Entity entity)
{
    return m_components[m_entityMap[entity]];
}

//removes component for specified entity if it exists
//Return values: 
//GRK_OK
//GRK_NOSUCHELEMENT : that entity has not componenet of that type
template<class ComponentType>
GRK_Result GRK_ComponentManager<ComponentType>::RemoveComponent(GRK_Entity entity)
{
    //check if the elment is in the map
    //and do what we need to if it is not
    auto it = m_entityMap.find(entity);
    if (it == m_entityMap.end())
    {
        return GRK_NOSUCHELEMENT;
    }
    else
    {
        //this entity exists so we move the last element of the components vector
        //to the spot that this one was taking up
        auto indexToMoveLastStoredComponent = m_entityMap[entity];
        auto lastElement = m_components.back();
        //use std::move so we cannibilize any allocated components and dont copy them
        //TODO 3 implement std::move constructor (&&) for GRK_Components
        m_components[indexToMoveLastStoredComponent] = std::move(lastElement);

        //then remove it from the map
        //and shorten our vector
        m_entityMap.erase(it);
        m_components.pop_back();
        return GRK_OK;
    }
}

// This function just iterates over every component and applies a function
template<class ComponentType>
GRK_Result GRK_ComponentManager<ComponentType>::ForEach(std::function<GRK_Result(ComponentType component)> lambda)
{
    auto result = GRK_OK;
    for (auto it = m_components.begin(); it != m_components.end(); ++it)
    {
        result |= lambda(*it);
    }

    return result;
}

template<class ComponentType>
GRK_Result GRK_ComponentManager<ComponentType>::GarbageCollection()
{
    //TODO 15 should i also keep a list of deletedbutnotcollected components directly in here and differentiate between removing it and cleaingin it?
    //TODO 20, be more clever, dont dlete every time! try bitsquid garbagecollection
    auto uncleanedEntities = m_world->GetDeletedUncleanedEntities();

    for (auto entity : uncleanedEntities)
    {
        auto instanceIterator = m_entityMap.find(entity);
        if (!instanceIterator == m_entityMap.end())
        {
            RemoveComponent(*instanceIterator);
        }
    }
}