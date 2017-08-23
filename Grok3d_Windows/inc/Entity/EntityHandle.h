/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __ENTITYHANDLER__H

#define __ENTITYHANDLER__H

#include "grok3d_types.h"
#include "EntityComponentManager.h"
#include "Component/ComponentHandle.h"

#include <type_traits>
#include <functional>

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

namespace Grok3d { namespace Entities 
{
    class GRK_EntityHandle
    {
    public:
        GRK_EntityHandle(Grok3d::GRK_EntityComponentManager* entityComponentManager, Grok3d::Entities::GRK_Entity entity);

        Grok3d::GRK_Result Destroy();
        bool inline IsDestroyed();

        template<class ComponentType>
        Grok3d::GRK_Result AddComponent(ComponentType& component);

        template<class ComponentType>
        Grok3d::GRK_Result RemoveComponent();

        template<class ComponentType>
        Grok3d::Components::GRK_ComponentHandle<ComponentType> GetComponent()
        {
            static_assert(
                std::is_base_of<GRK_Component, ComponentType>::value,
                "GRK_EntityHandle::GetComponent Method type param not base of GRK_Component");

            RETURN_FAILURE_IF_ENTITY_DESTROYED(
                Grok3d::Components::GRK_ComponentHandle<ComponentType>(nullptr, nullptr, -1),
                return m_manager->GetComponent<ComponentType>(m_entity););
        }

        bool HasComponents(Grok3d::Components::GRK_ComponentBitMask componentBits);

        bool operator==(const Grok3d::Entities::GRK_EntityHandle& rhs) const;

    private:
        friend ::std::hash<GRK_EntityHandle>;
        Grok3d::Entities::GRK_Entity m_entity;
        Grok3d::GRK_EntityComponentManager* m_manager;
    };
} /*Entities*/ } /*Grok3d*/

#endif