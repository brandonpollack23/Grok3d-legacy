#ifndef __TRANSLATIONCOMPONENT__H
#define __TRANSLATIONCOMPONENT__H

#include "../grok3d_types.h"

#include "Component.h"

namespace Grok3d { namespace Components 
{
    class GRK_TransformComponent : public GRK_Component
    {
    friend class GRK_ComponentManager<GRK_TransformComponent>;

    public:
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
