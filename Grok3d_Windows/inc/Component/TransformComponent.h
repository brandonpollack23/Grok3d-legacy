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
            void SetParent(GRK_TransformComponent* newParent);
            bool IsChildOf(GRK_TransformComponent* possibleParent);
            unsigned int GetSiblingIndex(); //use parent and iterate through the list looking for me
            void SetSiblingIndex(unsigned int newIndex); //use parent and iterate through the list looking for me, and move me

            bool HasChanged();
            int ChildCount();

            //TODO 30
            //glm::mat4 GetLocalToWorldTransformMatrix();
            //glm::mat4 GetWorldToLocalTransformMatrix();
            
            glm::vec3 SetWorldPosition();
            void GetWorldPosition(glm::vec3 v);

            glm::vec3 GetLocalPosition();
            void GetLocalPosition(glm::vec3 v);

            glm::quat GetLocalRotation();
            void GetLocalRotation(glm::quat q);

            glm::vec3 GetLocalScale();
            void GetLocalScale(glm::vec3 v);

            glm::quat GetWorldRotation();
            void SetWorldRotation(glm::quat q);

            //TODO 30
            //glm::vec3 GetForward();
            //void SetForward(glm::vec3 v);

            //glm::vec3 GetUp();
            //void SetUp(glm::vec3 v);

            //glm::vec3 GetRight();
            //void SetRight(glm::vec3 v);

            //TODO 25
            //void LookAt(glm::vec3 p);
            //void Rotate(glm::vec3 eulerAngles, CoordinateSpace relativeTo = CoordinateSpace::Self);
            //void RotateAround(glm::vec3 p, glm::vec3 axis, float angle);

            //void SetPositionAndRotation(glm::vec3 position, glm::quat rotation);

            //TODO 30
            //transforms this points/direction/vector coordinates to world coordinates or opposite for inverse
            //glm::vec3 TransformPoint(glm::vec3 p);
            //glm::vec3 TransformPointInverse(glm::vec3 p);

            //glm::vec3 TransformDirection(glm::vec3 direction);
            //glm::vec3 TransformDirectionInverse(glm::vec3 direction);

            //glm::vec3 TransformVector(glm::vec3 vector);
            //glm::vec3 TransformVectorInverse(glm::vec3 vector);

            //functionality
            void DetachChildren();
            GRK_TransformComponent* FindChildByName(std::string& name);
            GRK_TransformComponent* GetChild(unsigned int index);
        private:
            GRK_TransformComponent* parent;

            //a flag that is set if transform is changed, helpful in determining if new MVP is needed etc
            bool m_hasChanged;
            unsigned int m_childCount;

            //position, rotation, and scale relative to parent
            glm::vec3 m_localPosition;
            glm::quat m_localRotation;
            glm::vec3 m_localScale;
            glm::mat4 m_MVP;

            std::vector<GRK_TransformComponent*> m_children;
    };

    enum CoordinateSpace
    {
        Self,
        World
    };
} /*Components*/ } /*Grok3d*/

#endif
