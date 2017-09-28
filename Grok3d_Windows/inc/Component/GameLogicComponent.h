#ifndef __GAMELOGICCOMPONENT__H
#define __GAMELOGICCOMPONENT__H

#include "grok3d_types.h"
#include "Component/Component.h"
#include "Entity/EntityHandle.h"

#include <vector>

namespace Grok3d { namespace Components 
{
    class GRK_GameLogicComponent : public GRK_Component
    {
        public:
            //update function that takes in the change in time
            GRK_GameLogicComponent();

            void Update(double dt) const;

            typedef int BehaviourHandle;
            //returns the handle of the behaviour so it can be easily removed later
            BehaviourHandle RegisterBehaviour(GRK_GameBehaviourBase* behaviour);
            void UnregisterBehaviour(BehaviourHandle handle);

        protected:
            std::vector<GRK_GameBehaviourBase*> m_behaviours;
    };

    //this is one of the only classes heierarchies that uses dynamic dispatch (or heirarchies at all for that matter)
    // so that it can be overloaded and classes can be made in other languages later on (LUA or python)
    class GRK_GameBehaviourBase
    {
    public:
        GRK_GameBehaviourBase(Grok3d::Entities::GRK_EntityHandle owningEntity);

        virtual void Update(double dt) = 0;

    protected:
        Grok3d::Entities::GRK_EntityHandle m_owningEntity;
    };
} /*Components*/
} /*Grok3d*/

#endif
