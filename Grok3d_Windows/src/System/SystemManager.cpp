#include "System/SystemManager.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Systems;

GRK_SystemManager::GRK_SystemManager()
{
    //TODO initialize all the systems
    //m_systems = {};
}

GRK_Result GRK_SystemManager::UpdateSystemEntities(
        GRK_Entity entity, 
        GRK_ComponentBitMask newBitMask)
{
    GRK_Result result = GRK_OK;

    for(auto system : m_systems)
    {
        result |= system.UpdateSystemEntities(entity, newBitMask);
    }

    return result;
}

GRK_Result GRK_SystemManager::UpdateSystems(float dt)
{
    GRK_Result result;
    for(auto system : m_systems)
    {
        result |= system.Update(dt);
    }

    return result;
}
