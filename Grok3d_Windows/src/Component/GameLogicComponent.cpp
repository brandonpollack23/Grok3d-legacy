#include "Component\GameLogicComponent.h"

using namespace Grok3d;
using namespace Grok3d::Components;

void GRK_GameLogicComponent::Update(float dt)
{
    for (auto behaviour : m_behaviours)
    {
        behaviour->Update(dt);
    }
}

GRK_GameLogicComponent::BehaviourHandle GRK_GameLogicComponent::RegisterBehaviour(GRK_GameBehaviourBase* behaviour)
{
    m_behaviours.push_back(behaviour);
    return static_cast<BehaviourHandle>(m_behaviours.size() - 1);
}

void GRK_GameLogicComponent::UnregisterBehaviour(BehaviourHandle handle)
{
    if (handle > 0 && handle < m_behaviours.size())
    {
        m_behaviours.erase(m_behaviours.begin() + handle);
    }
}
