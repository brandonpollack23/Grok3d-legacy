#ifndef __ENGINE__H
#define __ENGINE__H

#include "grok3d_types.h"
#include "EntityComponentManager.h"
#include "System/SystemManager.h"

#include <functional>
#include <ctime>

namespace Grok3d 
{
    class GRK_Engine
    {
        public:
            GRK_Engine();

            //TODO overload initialize based on markup file
            Grok3d::GRK_Result Initialize();

            void Update(float dt);

            //function that never exits and runs update in an infinite loop until broken
            void Run();

            Grok3d::GRK_Result InjectInitialization(
                    std::function<Grok3d::GRK_Result(Grok3d::GRK_EntityComponentManager&)> initFunction);

        private:
            Grok3d::GRK_EntityComponentManager m_entityCompomnentManager;
            Grok3d::Systems::GRK_SystemManager m_systemManager;

            std::function<Grok3d::GRK_Result(Grok3d::GRK_EntityComponentManager&)> m_initFunction;
            //TODO add shouldContine for for loop and shouldReinitialize for Run func
    };
} /*Grok3d*/

#endif
