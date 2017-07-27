#ifndef __COMPONENTHANDLE__H
#define __COMPONENTHANDLE__H

#include "../grok3d_types.h"

namespace Grok3d { namespace Components
{
    template<class ComponentType>
    class GRK_ComponentHandle
    {
    static_assert(std::is_base_of<GRK_Component, ComponentType>::value, "The ComponentType passed to class GRK_ComponentHandle was not of base GRK_Component");
    public:
        Grok3d::Entities::GRK_Entity GetOwningEntity();

        ComponentType* operator::->();

        Grok3d::GRK_Result destroy();

    private:
        GRK_Entity owner;
        ComponentType* component;
        GRK_ComponentManager<ComponentType>* manager;
    };

} /*Components*/ } /*Grok3d*/

#endif