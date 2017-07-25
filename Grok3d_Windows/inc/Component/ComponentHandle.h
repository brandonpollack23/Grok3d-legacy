#ifndef __COMPONENTHANDLE__H
#define __COMPONENTHANDLE__H

#include "../grok3d_types.h"

namespace Grok3d { namespace Components
{
    template<class ComponentType>
    class ComponentHandle
    {
    public:
        Grok3d::Entities::GRK_Entity getOwningEntity();

        ComponentType* operator::->();

        Grok3d::GRK_Result destroy();

    private:
        GRK_Entity owner;
        ComponentType component;
        GRK_ComponentManager<ComponentType> manager;
    };

} /*Components*/ } /*Grok3d*/

#endif