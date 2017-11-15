#ifndef __SYSTEMMANAGER__H
#define __SYSTEMMANAGER__H

#include "grok3d_types.h"
#include "System/System.h"
#include "System/GameLogicSystem.h"

#include <array>

namespace Grok3d { namespace Systems
{
    class GRK_SystemManager
    {
    public:
        GRK_SystemManager();

        //thie function does a check to see if all the bits in it's bit mask 
        //are in it's bit mask
        auto UpdateSystemEntities(const Grok3d::Entities::GRK_EntityHandle& entity) -> Grok3d::GRK_Result;

        auto UnregisterEntity(const Grok3d::Entities::GRK_EntityHandle& entity) -> Grok3d::GRK_Result;

        auto UpdateSystems(double dt) -> Grok3d::GRK_Result;

        auto Render() const -> Grok3d::GRK_Result;

    private:
        std::array<GRK_System*, 1> m_systems;
        GRK_GameLogicSystem m_gls;
    };
} /*Systems*/ } /*Grok3d*/

#endif
