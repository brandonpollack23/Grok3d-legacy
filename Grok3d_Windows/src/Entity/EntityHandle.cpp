/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "Entity/EntityHandle.h"

#include "World.h"

#include "templates/is_valid.h"
#include "templates/enable_if.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

auto hasComponentBitOffset = Grok3d::Templates::is_valid([](auto&& x) -> decltype(x.ComponentBitOffset()) { });

GRK_TransformComponent test;

static_assert(decltype(hasComponentBitOffset(test))::value == true, "");

#define RETURN_FAILURE_IF_ENTITY_DESTROYED(error, statements) \
    do {\
        if (IsDestroyed()) \
        { \
            return error; \
        } \
        else \
        { \
            return statements; \
        } \
        return GRK_OK; \
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

using templates to require ComponentBitOffset static method
*/
//in case of fire use this
//template<class ComponentType>
//auto GRK_EntityHandle::AddComponent(ComponentType& component)
//-> typename Grok3d::Templates::enable_if<, GRK_Result>::type
template<class ComponentType>
GRK_Result GRK_EntityHandle::AddComponent(ComponentType& component)
{
    static_assert(decltype(hasComponentBitOffset(component))::value);
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
        GRK_NOSUCHENTITY,
        //can't have two components of the same type EXCEPT BEHAVIOUR COMPONENTS WHICH ARE SCRIPTS
        if (m_components & ComponentType.ComponentBitOffset() > 0 && ComponentType.ComponentBitOffset() != GRK_BEHAVIOURCOMPONENT_BITSHIFT)
        {
            return GRK_COMPONENTALREADYADDED;   
        }
        else
        {
            m_components |= ComponentType.ComponentBitOffset();
            return m_world->AddComponent<ComponentType>(m_entity, component);
        });
}

template<class ComponentType>
GRK_Result GRK_EntityHandle::RemoveComponent()
{
    static_assert(decltype(hasComponentBitOffset(component))::value);
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
        GRK_NOSUCHENTITY,
        if (m_components & ComponentType.ComponentBitOffset() == 0)
        {
            m_components &= ~ComponentType.ComponentBitOffset();
            return m_world->RemoveComponent<ComponentType>(m_entity); 
        }
        else
        {
            return GRK_NOSUCHCOMPONENTINENTITY;
        });
}

template<class ComponentType>
ComponentType* GRK_EntityHandle::GetComponent()
{
    static_assert(decltype(hasComponentBitOffset(component))::value);
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
        GRK_NOSUCHENTITY,
        if (m_components & ComponentType.ComponentBitOffset() == 0)
        {
            return m_world->GetComponent<ComponentType>(m_entity);
        }
        else
        {
            return nullptr;
        });
}


GRK_Result GRK_EntityHandle::Destroy()
{
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
    GRK_NOSUCHENTITY,
    m_entity = 0;
    m_components = 0;
    m_world->DeleteEntity(m_entity););
}

//convenience function to remove from bitmask but dont do full delete as it has already been called from this manager
template<class ComponentType>
GRK_Result GRK_EntityHandle::ComponentDestroyedByHandleCleanup()
{
    static_assert(decltype(hasComponentBitOffset(component))::value);
    m_components &= ~ComponentType.ComponentBitOffset()
}

bool inline GRK_EntityHandle::IsDestroyed()
{
    return m_entity == 0;
}

bool inline GRK_EntityHandle::HasComponents(int componentBits)
{
    RETURN_FAILURE_IF_ENTITY_DESTROYED(
        false,
        ((m_components & componentBits) == componentBits));
}