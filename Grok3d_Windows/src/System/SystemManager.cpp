/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/
#include "System/SystemManager.h"
#include "Entity/EntityHandle.h"
#include "System/System.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;
using namespace Grok3d::Systems;

GRK_SystemManager::GRK_SystemManager() noexcept :
    m_isInitialized(false),
    m_ecm(nullptr),
    m_gls(GRK_GameLogicSystem()),
    m_rs(GRK_RenderSystem())
{
    m_systems = { &m_gls };
}

//TODO fail if not initialized?
auto GRK_SystemManager::Initialize(GRK_EntityComponentManager* ecm) -> GRK_Result
{
    //save reference to ecm
    m_ecm = ecm;

    //initialize render system
    m_rs.Initialize(m_ecm);

    m_isInitialized = true;

    return GRK_Result::Ok;
}

auto GRK_SystemManager::UpdateSystemEntities(const GRK_EntityHandle& entity) -> GRK_Result
{
    auto result = GRK_Result::Ok;

    for(const auto& system : m_systems)
    {
        result |= system->UpdateSystemEntities(entity);
    }

    return result;
}

auto GRK_SystemManager::UnregisterEntity(const GRK_EntityHandle& entity) -> GRK_Result
{
    auto result = GRK_Result::Ok;

    for (auto system : m_systems)
    {
        result |= system->UnregisterEntity(entity);
    }

    return result;
}

auto GRK_SystemManager::UpdateSystems(const double dt) -> GRK_Result
{
    auto result = GRK_Result::Ok;
    for(const auto& system : m_systems)
    {
        result |= system->Update(dt);
    }

    return result;
}

auto GRK_SystemManager::Render() const -> GRK_Result
{
    return m_rs.Render();
}
