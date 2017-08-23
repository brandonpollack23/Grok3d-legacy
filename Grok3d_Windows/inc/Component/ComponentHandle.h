/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __COMPONENTHANDLE__H
#define __COMPONENTHANDLE__H

#include "../grok3d_types.h"

namespace Grok3d { namespace Components
{
    template<class ComponentType>
    class GRK_ComponentHandle
    {
    public:
        GRK_ComponentHandle(Grok3d::GRK_EntityComponentManager* entityComponentManager, ComponentType* component, Grok3d::Entities::GRK_Entity owner) :
            m_manager(entityComponentManager),
            m_component(component),
            m_owner(owner)
        {
            static_assert(std::is_base_of<GRK_Component, ComponentType>::value, "The ComponentType passed to class GRK_ComponentHandle was not of base GRK_Component");
        }


        Grok3d::Entities::GRK_Entity GetOwningEntity();

        ComponentType* operator->()
        {
            return m_component;
        }

        Grok3d::GRK_Result destroy();

    private:
        Grok3d::Entities::GRK_Entity m_owner;
        ComponentType* m_component;
        Grok3d::GRK_EntityComponentManager* m_manager;
    };

} /*Components*/ } /*Grok3d*/

#endif