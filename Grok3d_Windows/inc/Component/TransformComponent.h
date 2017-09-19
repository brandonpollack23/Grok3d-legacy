/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#ifndef __TRANSLATIONCOMPONENT__H
#define __TRANSLATIONCOMPONENT__H

#include "../grok3d_types.h"

#include <string>
#include <vector>

#include "Component.h"

#include "glm/fwd.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Grok3d { namespace Components 
{
    class GRK_TransformComponent : public GRK_Component
    {
        public:
            GRK_TransformComponent();
            //TODO as chaining lots of parents gets really big this will get slow, best to cache and have a dirty bit
            //TODO Constructors
            void SetParent(GRK_TransformComponent* newParent);
            void AttachChild(GRK_TransformComponent* newChild);
            bool IsChildOf(GRK_TransformComponent* possibleParent);
            unsigned int GetSiblingIndex(); //use parent and iterate through the list looking for me
            int GetChildIndex(GRK_TransformComponent* possibleChild);

            int ChildCount();

            glm::vec3 GetWorldPosition();
            void SetWorldPosition(glm::vec3 v);
            void SetWorldPosition(float x, float y, float z);

            glm::vec3 GetLocalPosition();
            void GetLocalPosition(glm::vec3 v);

            void TranslateLocal(glm::vec3 v);
            void TranslateLocal(float x, float y, float z);

            glm::vec3 GetLocalScale();
            void SetLocalScale(glm::vec3 v);
            void SetLocalScale(float x, float y, float z);

            //functionality
            void DetachChildren();
            GRK_TransformComponent* GetChild(unsigned int index);
        private:
            GRK_TransformComponent* m_parent;

            std::vector<GRK_TransformComponent*> m_children;

            //position, rotation, and scale relative to parent
            //TODO rotation as quaternion
            glm::vec3 m_localPosition;
            glm::vec3 m_localScale;
    };
} /*Components*/ } /*Grok3d*/

#endif
