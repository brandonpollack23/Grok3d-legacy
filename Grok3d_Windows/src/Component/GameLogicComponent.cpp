#include "grok3d_types.h"
#include "Component/GameLogicComponent.h"
#include "Entity/EntityHandle.h"

#include <vector>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;

GRK_GameLogicComponent::GRK_GameLogicComponent(GRK_EntityHandle entity) :
    m_entity(entity)
{
}

void GRK_GameLogicComponent::Update(float dt)
{
    for (const auto& behaviour : m_behaviours)
    {
        behaviour(m_entity, dt);
    }
}

GRK_GameLogicComponent::BehaviourHandle GRK_GameLogicComponent::RegisterBehaviour(Grok3d::GRK_Result (*behaviour)(Grok3d::Entities::GRK_EntityHandle, float))
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