
/**@file*/

#ifndef __ENGINE__H
#define __ENGINE__H

#include "grok3d_types.h"
#include "System/SystemManager.h"
#include "EntityComponentManager.h"

#include <functional>

namespace Grok3d 
{
    /**
     * @brief The game engine and main entry point to use Grok3d
     *
     * @details
     * This is the class that contains the
     * @link Grok3d::GRK_EntityComponentManager GRK_EntityComponentManager @endlink
     * and
     * @link Grok3d::Systems::GRK_SystemManager GRK_SystemManager @endlink
     *
     * It works through a relatively simple process:
     * 1) You intialize the state by passing a function that uses a reference to the internal
     * GRK_EntityComponentManager to set up your game/scene
     * 2) You update it in steps of time (for simulation purposes OR you just run it and it loops
     * forever*/
    class GRK_Engine
    {
        public:
            GRK_Engine();
            /**Construct and Initialize the engine in the same step
             *
             * @details
             * This works the same as calling the default constructor and subsequently calling 
             * @link Grok3d::GRK_Engine::InjectInitialization InjectInitialization @endlink and 
             * @link Grok3d::GRK_Engine::Initialize Initialize @endlink*/
            GRK_Engine(std::function<GRK_Result(Grok3d::GRK_EntityComponentManager&)> initFunction);

            //TODO overload initialize based on markup file
            /**Run the internally function inserted with 
             * @link Grok3d::GRK_Engine::InjectInitialization InjectInitialization @endlink*/
            auto Initialize() -> Grok3d::GRK_Result;

            /**Update all of the systems and advance the simulation forward one step of time
             * @param[in] dt the amount of time to step forward the simulation*/
            auto Update(double dt) -> void;

            /**Draw the scene*/
            auto Render() const -> void;

            /**Clean up deleted components*/
            auto GarbageCollect() -> void;

            /**function that never exits and runs update in an infinite loop until broken*/
            auto Run() -> void;

            /**Insert an intialzation function to set up the scene*/
            auto InjectInitialization(
                    std::function<Grok3d::GRK_Result(Grok3d::GRK_EntityComponentManager&)> initFunction) -> Grok3d::GRK_Result;

        private:
            Grok3d::GRK_EntityComponentManager m_entityComponentManager;
            Grok3d::Systems::GRK_SystemManager m_systemManager;

            /**An init function that sets up the scene by creating entities and adding components
             * using the @link Grok3d::GRK_EntityComponentManager GRK_EntityComponentManager @endlink
             * for an initial state*/
            std::function<Grok3d::GRK_Result(Grok3d::GRK_EntityComponentManager&)> m_initFunction;
    };
} /*Grok3d*/

#endif
