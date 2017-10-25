/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __TRANSLATIONCOMPONENT__H
#define __TRANSLATIONCOMPONENT__H

#include "../grok3d_types.h"

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
            void SetParent(GRK_TransformComponent* newParent);
            void AttachChild(GRK_TransformComponent* newChild);
            bool IsChildOf(const GRK_TransformComponent* const possibleParent) const;
            unsigned int GetSiblingIndex(); //use parent and iterate through the list looking for me
            int GetChildIndex(const GRK_TransformComponent* possibleChild) const;

            int ChildCount();

            glm::dvec3 GetWorldPosition() const;
            void SetWorldPosition(glm::dvec3 v);
            void SetWorldPosition(double x, double y, double z);

            glm::dvec3 GetLocalPosition() const;
            void GetLocalPosition(glm::dvec3 v);

            void TranslateLocal(glm::dvec3 v);
            void TranslateLocal(double x, double y, double z);

            glm::dvec3 GetLocalScale() const;
            void SetLocalScale(glm::dvec3 v);
            void SetLocalScale(double x, double y, double z);

            //functionality
            void DetachChildren();
            GRK_TransformComponent* GetChild(unsigned int index) const;
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
