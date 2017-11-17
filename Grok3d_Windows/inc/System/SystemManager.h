
/**@file*/

#ifndef __SYSTEMMANAGER__H
#define __SYSTEMMANAGER__H

#include "grok3d_types.h"
#include "System/System.h"
#include "System/GameLogicSystem.h"

#include <array>

namespace Grok3d { namespace Systems
{
    /**
     * @brief the manager for all systems to generalize interaction and act as a location for
     * storage of all the system classes*/
    class GRK_SystemManager
    {
    public:
        GRK_SystemManager();

        /**Forward the entity to all systems, they check if the bit mask of that entity mask that of
         * the system's requirments, if so they are added to the queue to be updated every frame*/
        auto UpdateSystemEntities(const Grok3d::Entities::GRK_EntityHandle& entity) -> Grok3d::GRK_Result;

        /**Unregisters the entity from all systems, if it is registered*/
        auto UnregisterEntity(const Grok3d::Entities::GRK_EntityHandle& entity) -> Grok3d::GRK_Result;

        /**Iterate through all systems and run their update functions*/
        auto UpdateSystems(double dt) -> Grok3d::GRK_Result;

        /**Call Render on the rendering system*/
        auto Render() const -> Grok3d::GRK_Result;

    private:
        std::array<GRK_System*, 1> m_systems; ///< an array for easy iteration of all systems, initialized on construction
        GRK_GameLogicSystem m_gls; ///< the game logic system
    };
} /*Systems*/ } /*Grok3d*/

#endif
