#ifndef __SYSTEMMANAGER__H
#define __SYSTEMMANAGER__H

#include "grok3d_types.h"
#include "System/System.h"

#include <array>

namespace Grok3d { namespace Systems
{
    class GRK_SystemManager
    {
    public:
        GRK_SystemManager();

        //thie function does a check to see if all the bits in it's bit mask 
        //are in it's bit mask
        Grok3d::GRK_Result UpdateSystemEntities(Grok3d::Entities::GRK_EntityHandle& entity);

        Grok3d::GRK_Result UnregisterEntity(Grok3d::Entities::GRK_EntityHandle& entity);

        Grok3d::GRK_Result UpdateSystems(float dt);

    private:
            std::array<GRK_System*, 1> m_systems;
    };
} /*Systems*/ } /*Grok3d*/

#endif
