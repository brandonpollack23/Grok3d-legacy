#include "System/GameLogicSystem.h"
#include "Entity/EntityHandle.h"
#include "Component/GameLogicComponent.h"
#include "Component/ComponentHandle.h"

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::Systems;

GRK_GameLogicSystem::GRK_GameLogicSystem() : GRK_System()
{
}

GRK_Result GRK_GameLogicSystem::Update(float dt)
{
    for (auto entity : m_trackedEntities)
    {
        auto logicComponent = entity.GetComponent<GRK_GameLogicComponent>();
        logicComponent->Update(dt);
    }

    return GRK_Result::Ok;
}

GRK_ComponentBitMask GRK_GameLogicSystem::GetComponentsBitMask()
{
    static GRK_ComponentBitMask mask = 
        GRK_Component::GetComponentTypeAccessIndex<GRK_GameLogicComponent>();

    return mask;
}