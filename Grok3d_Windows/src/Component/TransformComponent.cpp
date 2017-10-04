/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "Component/TransformComponent.h"

#include <vector>

using namespace Grok3d;
using namespace Grok3d::Components;

GRK_TransformComponent::GRK_TransformComponent() :
    m_parent(nullptr),
    m_children(std::vector<GRK_TransformComponent*>()),
    m_localPosition(glm::dvec3(0)),
    m_localScale(glm::dvec3(0))
{
}

void GRK_TransformComponent::SetParent(GRK_TransformComponent* newParent)
{
    m_parent = newParent;
    m_parent->AttachChild(this);
}

void GRK_TransformComponent::AttachChild(GRK_TransformComponent* newChild)
{
    this->m_children.push_back(newChild);
}

bool GRK_TransformComponent::IsChildOf(const GRK_TransformComponent* const possibleParent) const
{
    return possibleParent == m_parent;
}

unsigned int GRK_TransformComponent::GetSiblingIndex() //use parent and iterate through the list looking for me
{
    return m_parent->GetChildIndex(this);
}

int GRK_TransformComponent::GetChildIndex(const GRK_TransformComponent* const possibleChild) const
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
    return static_cast<int>(this->m_children.size());
}

glm::dvec3 GRK_TransformComponent::GetWorldPosition() const
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

void GRK_TransformComponent::SetWorldPosition(glm::dvec3 v)
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

void GRK_TransformComponent::SetWorldPosition(const double x, const double y, const double z)
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

glm::dvec3 GRK_TransformComponent::GetLocalPosition() const
{
    return m_localPosition;
}

void GRK_TransformComponent::GetLocalPosition(glm::dvec3 v)
{
    this->m_localPosition = v;
}

void GRK_TransformComponent::TranslateLocal(glm::dvec3 v)
{
    this->TranslateLocal(v.x, v.y, v.z);
}

void GRK_TransformComponent::TranslateLocal(const double x, const double y, const double z)
{
    m_localPosition.x += x;
    m_localPosition.y += y;
    m_localPosition.z += z;
}

glm::dvec3 GRK_TransformComponent::GetLocalScale() const
{
    return m_localScale;
}

void GRK_TransformComponent::SetLocalScale(glm::dvec3 v)
{
    m_localScale = v;
}

void GRK_TransformComponent::SetLocalScale(const double x, const double y, const double z)
{
    m_localScale.x = x;
    m_localScale.y = y;
    m_localScale.z = z;
}

void GRK_TransformComponent::DetachChildren()
{
    for (auto child : m_children)
    {
        child->SetParent(nullptr);
    }
    m_children.clear();
}

GRK_TransformComponent* GRK_TransformComponent::GetChild(const unsigned int index) const
{
    return (index < m_children.size() && index > 0) ? m_children[index] : nullptr;
}
