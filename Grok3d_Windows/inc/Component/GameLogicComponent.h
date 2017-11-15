/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

#ifndef __GAMELOGICCOMPONENT__H
#define __GAMELOGICCOMPONENT__H

#include "grok3d_types.h"

#include "EntityComponentManager.h"
#include "Entity/EntityHandle.h"

#include "bidir_map.h"

#include <vector>
#include <memory>

namespace Grok3d { namespace Components 
{
    class GRK_GameLogicComponent
    {
        public:
            typedef size_t BehaviourHandle;

        public:
            //update function that takes in the change in time
            GRK_GameLogicComponent();

            GRK_GameLogicComponent(GRK_GameLogicComponent&& glc);

            GRK_GameLogicComponent& operator=(GRK_GameLogicComponent&& rhs);

            auto Update(double dt) -> void;

            //returns the handle of the behaviour so it can be easily removed later
            auto RegisterBehaviour(std::unique_ptr<GRK_GameBehaviourBase> behaviour) -> BehaviourHandle;
            auto EnqueueBehaviourRemoval(BehaviourHandle handle) -> Grok3d::GRK_Result;

        protected:
            auto UnregisterBehaviour(BehaviourHandle handle) -> Grok3d::GRK_Result;

        protected:
            typedef size_t BehaviourIndex;

            static BehaviourHandle s_nextHandle;

            notstd::unordered_bidir_map<BehaviourHandle, BehaviourIndex> m_behaviourIndexMap;
            std::vector<std::unique_ptr<GRK_GameBehaviourBase>> m_behaviours;

            std::vector<BehaviourHandle> m_behavioursToRemove;
    };

    // this is one of the only classes heierarchies that uses dynamic dispatch (or heirarchies at all for that matter)
    // so that it can be overloaded and classes can be made in other languages later on (LUA or python)
    class GRK_GameBehaviourBase
    {
    public:
        GRK_GameBehaviourBase(Grok3d::Entities::GRK_EntityHandle owningEntity);

        virtual void Update(double dt) = 0;

    protected:
        Grok3d::GRK_Result UnregisterThisBehaviour();

    protected:
        friend class GRK_GameLogicComponent;

        Grok3d::Entities::GRK_EntityHandle m_owningEntity;
        GRK_GameLogicComponent::BehaviourHandle m_behaviourHandle;
    };
} /*Components*/
} /*Grok3d*/

#endif
