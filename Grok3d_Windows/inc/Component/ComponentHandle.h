/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __COMPONENTHANDLE__H
#define __COMPONENTHANDLE__H

#include "grok3d_types.h"
#include "Component/Component.h"

namespace Grok3d { namespace Components
{
    template<class ComponentType>
    class GRK_ComponentHandle
    {
    public:
        GRK_ComponentHandle(
                const Grok3d::GRK_EntityComponentManager* entityComponentManager,
                const ComponentType* component,
                const Grok3d::Entities::GRK_Entity owner) :
            m_manager(entityComponentManager),
            m_component(component),
            m_owner(owner)
        {
            static_assert(std::is_base_of<GRK_Component, ComponentType>::value, "The ComponentType passed to class GRK_ComponentHandle was not of base GRK_Component");
        }

        const Grok3d::Entities::GRK_Entity GetOwningEntity() const
        {
            return m_owner;
        }

        bool IsHandleValid() const
        {
            Grok3d::Components::GRK_ComponentBitMask thisComponentBitMask = IndexToMask(Grok3d::Components::GRK_Component::GetComponentTypeAccessIndex<ComponentType>());
            Grok3d::Components::GRK_ComponentBitMask components = m_manager->GetEntityComponentsBitMask(m_owner);
            return (components & thisComponentBitMask) == thisComponentBitMask;
        }

        ComponentType* operator->()
        {
            if (IsHandleValid())
            {
                return const_cast<ComponentType*>(m_component);
            }
            else
            {
                return nullptr;
            }
        }

        Grok3d::GRK_Result Destroy() const
        {
            return m_manager->RemoveComponent(m_entity);
        }

    private:
        const Grok3d::Entities::GRK_Entity m_owner;
        const ComponentType* m_component;
        const Grok3d::GRK_EntityComponentManager* m_manager;
    };

    //template specialization for GRK_TransformComponent, every entity MUST have this it cannot be destroyed
    template<>
    GRK_Result GRK_ComponentHandle<GRK_TransformComponent>::Destroy() const
    {
        //TODO 20 log that this cant happen
        return GRK_Result::Ok;
    }

} /*Components*/ } /*Grok3d*/

#endif
