#include "grok3d.h"
#include "System/System.h"
#include "Entity/EntityHandle.h"

#include <unordered_set>

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Components;
using namespace Grok3d::Systems;

GRK_System::GRK_System()
{
    m_trackedEntities = std::unordered_set<GRK_EntityHandle>(INITIAL_ENTITY_ARRAY_SIZE);
}

GRK_Result GRK_System::Update(double dt)
{
    GRK_Result result = GRK_Result::Ok;
    result |= UpdateInternal(dt);
    result |= CompleteUnregisterEntities();
    return result;
}

GRK_Result GRK_System::UpdateSystemEntities(const GRK_EntityHandle& entity)
{
    GRK_ComponentBitMask myMask = GetComponentsBitMask();

    //if mask has all components I need
    if (entity.HasComponents(myMask))
    {
        try
        {
            m_trackedEntities.insert(entity);
        }
        catch(...)
        {
            return GRK_Result::ErrorAddingToSystem;
        }
    }
    else
    {
        auto itRemove = m_trackedEntities.find(entity);
        if (itRemove != m_trackedEntities.end())
        {
            m_trackedEntities.erase(entity);
        }
    }

    return GRK_Result::Ok;
}

GRK_Result GRK_System::UnregisterEntity(const GRK_EntityHandle& entity)
{
    m_entitiesToUnregister.push_back(entity);

    return GRK_Result::Ok;
}

Grok3d::GRK_Result GRK_System::CompleteUnregisterEntities()
{
    GRK_Result result = GRK_Result::Ok;
    for (auto& entity : m_entitiesToUnregister)
    {
        auto entityIt = m_trackedEntities.find(entity);
        if (entityIt != m_trackedEntities.end())
        {
            m_trackedEntities.erase(entityIt);
        }
        else
        {
            result = GRK_Result::NoSuchEntity;
        }
    }

    return result;
}
