#ifndef __GAMELOGICSYSTEM__H
#define __GAMELOGICSYSTEM__H

#include "grok3d_types.h"
#include "System/System.h"

namespace Grok3d { namespace Systems
{
    class GRK_GameLogicSystem : public GRK_System
    {
        public:
            GRK_GameLogicSystem();
            auto UpdateInternal(double dt) override -> GRK_Result;
        
        protected:
            auto GetComponentsBitMask() const override -> Grok3d::Components::GRK_ComponentBitMask;
    };
} /*Systems*/ } /*Grok3d*/

#endif
