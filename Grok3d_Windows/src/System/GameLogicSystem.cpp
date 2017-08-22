#include "System/GameLogicSystem.h"

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::Systems;

GRK_GameLogicSystem::GRK_GameLogicSystem() : GRK_System()
{
}

void GRK_GameLogicSystem::Update(float dt)
{
    for (auto entity : m_trackedEntities)
    {
        GRK_GameLogicComponent logicComponent = entity.GetComponent<GRK_GameLogicComponent>();
        logicComponent.Update(dt);
    }
}

GRK_ComponentBitMask GetComponentsBitMask()
{
    static GRK_ComponentBitMask mask = 
        GRK_Component::GetComponentTypeAccessIndex<GRK_GameLogicComponent>();

    return mask;
}
