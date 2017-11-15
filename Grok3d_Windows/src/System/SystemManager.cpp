#include "System/SystemManager.h"
#include "Entity/EntityHandle.h"
#include "System/System.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;
using namespace Grok3d::Systems;

GRK_SystemManager::GRK_SystemManager()
{
    //TODO initialize all the systems
    m_systems = { &m_gls };
    //TODO initialize RENDER SYSTEM which is managed seperate so simulation and render can be decoupled
}

auto GRK_SystemManager::UpdateSystemEntities(const GRK_EntityHandle& entity) -> GRK_Result
{
    GRK_Result result = GRK_Result::Ok;

    for(const auto& system : m_systems)
    {
        result |= system->UpdateSystemEntities(entity);
    }

    return result;
}

auto GRK_SystemManager::UnregisterEntity(const GRK_EntityHandle& entity) -> GRK_Result
{
    GRK_Result result = GRK_Result::Ok;

    for (auto system : m_systems)
    {
        result |= system->UnregisterEntity(entity);
    }

    return result;
}

auto GRK_SystemManager::UpdateSystems(const double dt) -> GRK_Result
{
    GRK_Result result = GRK_Result::Ok;
    for(const auto& system : m_systems)
    {
        result |= system->Update(dt);
    }

    return result;
}

auto GRK_SystemManager::Render() const -> GRK_Result
{
    //TODO call draw system render function
    return GRK_Result::Ok;
}
