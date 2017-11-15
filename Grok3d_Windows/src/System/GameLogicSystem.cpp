#include "System/GameLogicSystem.h"
#include "Entity/EntityHandle.h"
#include "Component/GameLogicComponent.h"
#include "Component/ComponentHandle.h"
#include "EntityComponentManager.h"

using namespace Grok3d;
using namespace Grok3d::Components;
using namespace Grok3d::Systems;

GRK_GameLogicSystem::GRK_GameLogicSystem() : GRK_System()
{
}

auto GRK_GameLogicSystem::UpdateInternal(const double dt) -> GRK_Result
{
    for (const auto& entity : m_trackedEntities)
    {
        auto logicComponent = entity.GetComponent<GRK_GameLogicComponent>();
        logicComponent->Update(dt);
    }

    return GRK_Result::Ok;
}

auto GRK_GameLogicSystem::GetComponentsBitMask() const -> GRK_ComponentBitMask
{
    static GRK_ComponentBitMask mask = 
        IndexToMask(GRK_EntityComponentManager::GetComponentTypeAccessIndex<GRK_GameLogicComponent>());

    return mask;
}
