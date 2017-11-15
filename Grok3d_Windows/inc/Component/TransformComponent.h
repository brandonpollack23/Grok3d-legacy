/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __TRANSLATIONCOMPONENT__H
#define __TRANSLATIONCOMPONENT__H

#include "grok3d_types.h"

#include <string>
#include <vector>

#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Grok3d { namespace Components 
{
    class GRK_TransformComponent
    {
        public:
            GRK_TransformComponent();
            //TODO as chaining lots of parents gets really big this will get slow, best to cache and have a dirty bit
            auto SetParent(GRK_TransformComponent* newParent) -> void;
            auto AttachChild(GRK_TransformComponent* newChild) -> void;
            auto IsChildOf(const GRK_TransformComponent* const possibleParent) const -> bool;
            auto GetSiblingIndex() -> unsigned int; //use parent and iterate through the list looking for me
            auto GetChildIndex(const GRK_TransformComponent* possibleChild) const -> int;

            auto ChildCount() -> int;

            auto GetWorldPosition() const -> glm::dvec3;
            auto SetWorldPosition(glm::dvec3 v) -> void;
            auto SetWorldPosition(double x, double y, double z) -> void;

            auto GetLocalPosition() const -> glm::dvec3;
            auto GetLocalPosition(glm::dvec3 v) -> void;

            auto TranslateLocal(glm::dvec3 v) -> void;
            auto TranslateLocal(double x, double y, double z) -> void;

            auto GetLocalScale() const -> glm::dvec3;
            auto SetLocalScale(glm::dvec3 v) -> void;
            auto SetLocalScale(double x, double y, double z) -> void;

            //functionality
            auto DetachChildren() -> void;
            auto GetChild(unsigned int index) const -> GRK_TransformComponent*;
        private:
            GRK_TransformComponent* m_parent;

            std::vector<GRK_TransformComponent*> m_children;

            //position, rotation, and scale relative to parent
            //TODO rotation as quaternion
            glm::dvec3 m_localPosition;
            glm::dvec3 m_localScale;
    };
} /*Components*/ } /*Grok3d*/

#endif
