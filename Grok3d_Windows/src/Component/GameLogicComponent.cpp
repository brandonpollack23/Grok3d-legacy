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
    if (&glc != this)
    {
        m_behaviours = std::move(glc.m_behaviours);
        m_behaviourIndexMap = std::move(glc.m_behaviourIndexMap);
    }
}

GRK_GameLogicComponent& GRK_GameLogicComponent::operator=(GRK_GameLogicComponent&& rhs)
{
    m_behaviours = std::move(rhs.m_behaviours);
    return *this;
}

void GRK_GameLogicComponent::Update(double dt)
{
    if (m_behavioursToRemove.size() > 0)
    {
        for (const auto& behaviour : m_behavioursToRemove)
        {
            UnregisterBehaviour(behaviour);
        }

        m_behavioursToRemove.clear();
    }
    for (const auto& behaviour : m_behaviours)
    {
        behaviour->Update(dt);
    }
}

GRK_GameLogicComponent::BehaviourHandle GRK_GameLogicComponent::RegisterBehaviour(std::unique_ptr<GRK_GameBehaviourBase> behaviour)
{
    behaviour->m_behaviourHandle = s_nextHandle;
    m_behaviours.push_back(std::move(behaviour));
    m_behaviourIndexMap.put(s_nextHandle, (m_behaviours.size() - 1));
    return s_nextHandle++; 
}

GRK_Result GRK_GameLogicComponent::EnqueueBehaviourRemoval(const GRK_GameLogicComponent::BehaviourHandle handle)
{
    m_behavioursToRemove.push_back(handle);

    return GRK_Result::Ok;
}

GRK_Result GRK_GameLogicComponent::UnregisterBehaviour(const BehaviourHandle handle)
{
    auto removeIndex = m_behaviourIndexMap.at(handle);

    auto backHandle = m_behaviourIndexMap.reverse_at(m_behaviours.size() - 1);

    m_behaviours[removeIndex] = std::move(m_behaviours.back());
    m_behaviours.pop_back();

    m_behaviourIndexMap.erase(handle);

    if (m_behaviours.size() > 0)
    {
        m_behaviourIndexMap.reverse_erase(m_behaviours.size() - 1);
        m_behaviourIndexMap.put(backHandle, removeIndex);
    }

    return GRK_Result::Ok;
}

GRK_GameBehaviourBase::GRK_GameBehaviourBase(GRK_EntityHandle owningEntity) :
    m_owningEntity(owningEntity)
{
}

Grok3d::GRK_Result GRK_GameBehaviourBase::UnregisterThisBehaviour()
{
    return m_owningEntity.GetComponent<GRK_GameLogicComponent>()->EnqueueBehaviourRemoval(m_behaviourHandle);
}
