/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __COMPONENTHANDLE__H
#define __COMPONENTHANDLE__H

#include "grok3d_types.h"

namespace Grok3d { namespace Components
{
    template<class ComponentType, class ECM>
    class GRK_ComponentHandle
    {
    public:
        GRK_ComponentHandle(
                const ECM* entityComponentManager,
                const ComponentType* component,
                const Grok3d::Entities::GRK_Entity owner) :
            m_owner(owner),
            m_component(component),
            m_manager(entityComponentManager)
        {
        }

        auto GetOwningEntity() const -> const Grok3d::Entities::GRK_Entity
        {
            return m_owner;
        }

        auto IsHandleValid() const -> bool
        {
            auto thisComponentBitMask = IndexToMask(ECM::template GetComponentTypeAccessIndex<ComponentType>());
            auto components = m_manager->GetEntityComponentsBitMask(m_owner);
            return ((components & thisComponentBitMask) == thisComponentBitMask);
        }

        auto operator->() -> ComponentType*
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

        auto Destroy() const -> Grok3d::GRK_Result
        {
            return m_manager->RemoveComponent(m_owner);
        }

    private:
        const Grok3d::Entities::GRK_Entity m_owner;
        const ComponentType* m_component;
        const ECM* m_manager;
    };
} /*Components*/ } /*Grok3d*/

#endif
