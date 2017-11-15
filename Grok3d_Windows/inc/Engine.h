#ifndef __ENGINE__H
#define __ENGINE__H

#include "grok3d_types.h"
#include "System/SystemManager.h"
#include "EntityComponentManager.h"

#include <functional>

namespace Grok3d 
{
    class GRK_Engine
    {
        public:
            GRK_Engine();
            GRK_Engine(std::function<GRK_Result(Grok3d::GRK_EntityComponentManager&)> initFunction);

            //TODO overload initialize based on markup file
            auto Initialize() -> Grok3d::GRK_Result;

            auto Update(double dt) -> void;
            auto Render() const -> void;
            auto GarbageCollect() -> void;

            //function that never exits and runs update in an infinite loop until broken
            auto Run() -> void;

            auto InjectInitialization(
                    std::function<Grok3d::GRK_Result(Grok3d::GRK_EntityComponentManager&)> initFunction) -> Grok3d::GRK_Result;

        private:
            Grok3d::GRK_EntityComponentManager m_entityComponentManager;
            Grok3d::Systems::GRK_SystemManager m_systemManager;

            std::function<Grok3d::GRK_Result(Grok3d::GRK_EntityComponentManager&)> m_initFunction;
    };
} /*Grok3d*/

#endif
