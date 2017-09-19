#include "Component\GameLogicComponent.h"
#include "grok3d_types.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

GRK_GameBehaviourBase::GRK_GameBehaviourBase(GRK_Entity owningEntity, GRK_EntityComponentManager* entityComponentManager) :
    m_owningEntity(owningEntity),
    m_entityComponentManager(entityComponentManager)
{
}


void GRK_GameLogicComponent::Update(float dt)
{
    for (const auto& behaviour : m_behaviours)
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
