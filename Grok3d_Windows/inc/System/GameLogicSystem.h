#ifndef __GAMELOGICSYSTEM__H
#define __GAMELOGICSYSTEM__H

#include "grok3d_types.h"
#include "System/System.h"

namespace Grok3d { namespace Systems
{
    class GRK_GameLogicSystem : public GRK_System
    {
        public:
            GRK_GameLogicSystem::GRK_GameLogicSystem();
            virtual GRK_Result Update(float dt) override;
        
        protected:
            virtual Grok3d::Components::GRK_ComponentBitMask GetComponentsBitMask() override;
    };
} /*Systems*/ } /*Grok3d*/

#endif
