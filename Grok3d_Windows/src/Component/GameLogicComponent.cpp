#include "Component\GameLogicComponent.h"
#include "grok3d_types.h"

#include <vector>
#include <memory>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

GRK_GameLogicComponent::BehaviourHandle GRK_GameLogicComponent::s_nextHandle = 1;

GRK_GameLogicComponent::GRK_GameLogicComponent() :
    m_behaviours(std::vector<std::unique_ptr<GRK_GameBehaviourBase>>())
{
}

GRK_GameLogicComponent::GRK_GameLogicComponent(GRK_GameLogicComponent&& glc)
{
    m_behaviours = std::move(glc.m_behaviours);
}

GRK_GameLogicComponent& GRK_GameLogicComponent::operator=(GRK_GameLogicComponent&& rhs)
{
    m_behaviours = std::move(rhs.m_behaviours);
    return *this;
}

void GRK_GameLogicComponent::Update(double dt) const
{
    for (const auto& behaviour : m_behaviours)
    {
        behaviour->Update(dt);
    }
}

GRK_GameLogicComponent::BehaviourHandle GRK_GameLogicComponent::RegisterBehaviour(std::unique_ptr<GRK_GameBehaviourBase> behaviour)
{
    m_behaviours.push_back(std::move(behaviour));
    m_behaviourIndexMap[s_nextHandle] = m_behaviours.size() - 1;
    return s_nextHandle++; 
}

void GRK_GameLogicComponent::UnregisterBehaviour(BehaviourHandle handle)
{
    auto removeIndex = m_behaviourIndexMap[handle];

    m_behaviours[removeIndex] = std::move(m_behaviours.back());
    m_behaviours.pop_back();

    m_behaviourIndexMap[handle] = removeIndex;
}

GRK_GameBehaviourBase::GRK_GameBehaviourBase(GRK_EntityHandle owningEntity) :
    m_owningEntity(owningEntity)
{
}
