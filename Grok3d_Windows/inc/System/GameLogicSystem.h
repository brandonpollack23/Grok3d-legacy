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
            auto UpdateInternal(double dt) -> GRK_Result override;
        
        protected:
            auto GetComponentsBitMask() const -> Grok3d::Components::GRK_ComponentBitMask override;
    };
} /*Systems*/ } /*Grok3d*/

#endif
