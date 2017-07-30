/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "Entity/EntityHandle.h"

#include "World.h"

#include "Assertions.h"

using namespace Grok3d;
using namespace Grok3d::Assertions;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

#define RETURN_FAILURE_IF_ENTITY_DESTROYED(error, statements) \
    do {\
        if (IsDestroyed()) \
        { \
            return error; \
        } \
        else \
        { \
            statements; \
        } \
    } while(0)

GRK_EntityHandle::GRK_EntityHandle(GRK_World* world, GRK_Entity entity) : 
    m_world(world),
    m_entity(entity)
{
}

/** AddComponent
This function adds a component to the system and forwards to the entire world,
including the respective component manager, 
which makes it registerd to the game

saying entity.addcomponent is just more intuitive than world.getmanager().addcomponent(entity, component)

using templates to require GetComponentTypeAccessIndex static method
*/
//in case of fire use this
//template<class ComponentType>
//auto GRK_EntityHandle::AddComponent(ComponentType& component)
//-> typename Grok3d::Templates::enable_if<, GRK_Result>::type
template<class ComponentType>
GRK_Result GRK_EntityHandle::AddComponent(ComponentType& component)
{
    static_assert(decltype(hasComponentTypeAccessIndex(component))::value, "GRK_EntityHandle::AddComponent Method type param not base of GRK_Component");
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
        GRK_NOSUCHENTITY,
        //can't have two components of the same type
        // TODO 10 find a way to make it so we can have more than one BehaviourComponent
        return m_world->AddComponent<ComponentType>(m_entity, component););
}

template<class ComponentType>
GRK_Result GRK_EntityHandle::RemoveComponent()
{
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
        GRK_NOSUCHENTITY,
        return m_world->RemoveComponent<ComponentType>(m_entity););
}

template<class ComponentType>
ComponentType* GRK_EntityHandle::GetComponent()
{
    static_assert(decltype(hasComponentTypeAccessIndex(component))::value, "GRK_EntityHandle::GetComponent Method type param not base of GRK_Component");
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
        GRK_NOSUCHENTITY,
        return m_world->GetComponent<ComponentType>(m_entity););
}


GRK_Result GRK_EntityHandle::Destroy()
{
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
    GRK_Result::NoSuchEntity,
    m_entity = 0;
    return m_world->DeleteEntity(m_entity););
}

bool inline GRK_EntityHandle::IsDestroyed()
{
    return m_entity == 0;
}

bool inline GRK_EntityHandle::HasComponents(int componentBits)
{
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
        false,
        GRK_ComponentBitMask components = m_world->GetEntityComponentsBitMask(m_entity);
        return ((components & componentBits) == componentBits));
}