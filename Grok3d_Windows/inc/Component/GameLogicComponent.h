#ifndef __GAMELOGICCOMPONENT__H
#define __GAMELOGICCOMPONENT__H

#include "grok3d_types.h"
#include "Component/Component.h"
#include "Entity/EntityHandle.h"

#include <vector>
#include <memory>

namespace Grok3d { namespace Components 
{
    class GRK_GameLogicComponent : public GRK_Component
    {
        public:
            typedef size_t BehaviourHandle;

        public:
            //update function that takes in the change in time
            GRK_GameLogicComponent();

            GRK_GameLogicComponent(GRK_GameLogicComponent&& glc);

            GRK_GameLogicComponent& operator=(GRK_GameLogicComponent&& rhs);

            void Update(double dt) const;

            //returns the handle of the behaviour so it can be easily removed later
            BehaviourHandle RegisterBehaviour(std::unique_ptr<GRK_GameBehaviourBase> behaviour);
            void UnregisterBehaviour(BehaviourHandle handle);

        protected:
            typedef size_t BehaviourIndex;

            static BehaviourHandle s_nextHandle;

            std::unordered_map<BehaviourHandle, BehaviourIndex> m_behaviourIndexMap;
            std::vector<std::unique_ptr<GRK_GameBehaviourBase>> m_behaviours;
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
