/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "Component/TransformComponent.h"

using namespace Grok3d;
using namespace Grok3d::Components;

void GRK_TransformComponent::SetParent(GRK_TransformComponent* newParent)
{
    m_parent = newParent;
    m_parent->AttachChild(this);
}

void GRK_TransformComponent::AttachChild(GRK_TransformComponent* newChild)
{
    this->m_children.push_back(newChild);
}

bool GRK_TransformComponent::IsChildOf(GRK_TransformComponent* possibleParent)
{
    return possibleParent == m_parent;
}

unsigned int GRK_TransformComponent::GetSiblingIndex() //use parent and iterate through the list looking for me
{
    return m_parent->GetChildIndex(this);
}

int GRK_TransformComponent::GetChildIndex(GRK_TransformComponent* possibleChild)
{
    for (int i = 0; i < m_children.size(); ++i)
    {
        if (m_children[i] == possibleChild) 
        {
            return i;
        }
    }
    return -1;
}

int GRK_TransformComponent::ChildCount()
{
    return this->m_children.size();
}

glm::vec3 GRK_TransformComponent::GetWorldPosition()
{
    //if i have no parent, my position is world position
    //otherwise my position concatonated with my parents' world position (recursive) is my actual position
    if (m_parent == nullptr)
    {
        return m_localPosition;
    }
    else
    {
        return m_localPosition + m_parent->GetWorldPosition();
    }
}

void GRK_TransformComponent::SetWorldPosition(glm::vec3 v)
{
    //if no parent, setting world position is setting my position
    //otherwise it is these coordinates subtracted from my parent's position recursively
    if (m_parent = nullptr)
    {
        m_localPosition = v;
    }
    else
    {
        m_localPosition = v - m_parent->GetWorldPosition();
    }
}

void GRK_TransformComponent::SetWorldPosition(float x, float y, float z)
{
    //if no parent, setting world position is setting my position
    //otherwise it is these coordinates subtracted from my parent's position recursively
    if (m_parent = nullptr)
    {
        m_localPosition.x = x;
        m_localPosition.y = y;
        m_localPosition.z = z;
    }
    else
    {
        m_localPosition.x = x - m_parent->GetWorldPosition().x;
        m_localPosition.y = y - m_parent->GetWorldPosition().y;
        m_localPosition.z = z - m_parent->GetWorldPosition().z;
    }
}

glm::vec3 GRK_TransformComponent::GetLocalPosition()
{
    return m_localPosition;
}

void GRK_TransformComponent::GetLocalPosition(glm::vec3 v)
{
    this->m_localPosition = v;
}


glm::vec3 GRK_TransformComponent::GetLocalScale()
{
    return m_localScale;
}

void GRK_TransformComponent::SetLocalScale(glm::vec3 v)
{
    m_localScale = v;
}

void GRK_TransformComponent::SetLocalScale(float x, float y, float z)
{
    m_localScale.x = x;
    m_localScale.y = y;
    m_localScale.z = z;
}

