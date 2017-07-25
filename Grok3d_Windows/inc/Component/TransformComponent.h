#ifndef __TRANSLATIONCOMPONENT__H
#define __TRANSLATIONCOMPONENT__H

#include "../grok3d_types.h"

namespace Grok3d { namespace Components 
{
    class GRK_TransformComponent
    {
    friend class GRK_ComponentManager<GRK_TransformComponent>;

    public:
        constexpr int ComponentBitOffset() { return static_cast<const int>(GRK_Component_BitShifts::TransformComponent); };

        GRK_ComponentManager<GRK_TransformComponent>* GetComponentManager();
    private:
        //private so only the componentmanager you register to can set this
        GRK_ComponentManager<GRK_TransformComponent>* componentManager;
        //TODO 3 constructors
        //TODO 3 methods
        //TODO 3 members
    };
} /*Components*/ } /*Grok3d*/

#endif
