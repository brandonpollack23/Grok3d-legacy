/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3d@gmail.com
* This file is available under the MIT license included in the project
*/

/**@file*/

#ifndef __SYSTEMMANAGER__H
#define __SYSTEMMANAGER__H

#include "grok3d_types.h"
#include "System/System.h"
#include "System/GameLogicSystem.h"
#include "System/RenderSystem.h"

#include <array>

namespace Grok3d { namespace Systems
{
    /**
     * @brief the manager for all systems to generalize interaction and act as a location for
     * storage of all the system classes*/
    class GRK_SystemManager
    {
    public:
        GRK_SystemManager() noexcept;

        /**Initializes internal class with the ECM and sets up any classes that depend on it
         * internally*/
        auto Initialize(Grok3d::GRK_EntityComponentManager* ecm) -> GRK_Result;

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
        bool m_isInitialized;

        Grok3d::GRK_EntityComponentManager* m_ecm;

        std::array<GRK_System*, 1> m_systems; ///< an array for easy iteration of all systems, initialized on construction
        GRK_GameLogicSystem m_gls; ///< the game logic system
        GRK_RenderSystem m_rs; ///< the rendering system
    };
} /*Systems*/ } /*Grok3d*/

#endif
