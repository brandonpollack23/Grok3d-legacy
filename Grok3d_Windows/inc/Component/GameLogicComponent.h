#ifndef __GAMELOGICCOMPONENT__H
#define __GAMELOGICCOMPONENT__H

#include "grok3d_types.h"
#include "Entity/EntityHandle.h"
#include "Component/Component.h"

#include <vector>

namespace Grok3d { namespace Components 
{
    class GRK_GameLogicComponent : public GRK_Component
    {
        public:
            //update function that takes in the change in time
            GRK_GameLogicComponent(Grok3d::Entities::GRK_EntityHandle entity);

            void Update(float dt);

            typedef int BehaviourHandle;
            //returns the handle of the behaviour so it can be easily removed later
            BehaviourHandle RegisterBehaviour(Grok3d::GRK_Result (*behaviour)(Grok3d::Entities::GRK_EntityHandle, float));
            void UnregisterBehaviour(BehaviourHandle handle);

        protected:
            std::vector<Grok3d::GRK_Result (*)(Grok3d::Entities::GRK_EntityHandle, float)> m_behaviours;
            Grok3d::Entities::GRK_EntityHandle m_entity;
    };
} /*Components*/ } /*Grok3d*/

#endif