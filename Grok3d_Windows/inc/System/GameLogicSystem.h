#ifndef __GAMELOGICSYSTEM__H
#define __GAMELOGICSYSTEM__H

#include "grok3d_types.h"

namespace Grok3d { namespace Systems
{
    class GRK_GameLogicSystem : public GRK_System
    {
        public:
            GRK_GameLogicSystem::GRK_GameLogicSystem();
            virtual void Update(float dt) override;
        
        protected:
            virtual Grok3d::Component::GRK_ComponentBitMask GetComponentsBitMask() override;
    };
} /*Systems*/ } /*Grok3d*/

#endif
