#include "System/system.h"
#include "grok3d.h"

using namespace Grok3d;
using namespace Grok3d::Entities;
using namespace Grok3d::Systems;

GRK_System::GRK_System()
{
    m_trackedEntities = new std::unordered_set<GRK_Entity>(INITIAL_ENTITY_ARRAY_SIZE);
}

GRK_Result GRK_System::UpdateSystemEntities(GRK_Entity entity, GRK_ComponentBitMask newBitMask)
{
    GRK_ComponentBitMask myMask = GetComponentsBitMask();

    //if mask has all components I need
    if ((myMask & newBitMask) == newBitMask)
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
            m_trackedEntities.remove(entity);
        }
    }

    return GRK_OK;
}
