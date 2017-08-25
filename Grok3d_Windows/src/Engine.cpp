#include "Engine.h"
#include "entityComponentManager.h"
#include "System/SystemManager.h"

#include <ctime>

using namespace Grok3d;
using namespace Grok3d::Systems;

GRK_Engine::GRK_Engine()
{
    //Inject systemManager dependency so we can update the systems
    m_entityCompomnentManager.Initialize(&m_systemManager);
}

GRK_Result GRK_Engine::Initialize()
{
    if(m_initFunction)
    {
        return m_initFunction(m_entityCompomnentManager);
    }
    else
    {
        return GRK_Result::EngineFailureNoInitialState;
    }
}

void GRK_Engine::Update(float dt)
{
    m_systemManager.Update(dt);
}

void GRK_Engine::Run()
{
    if(Initialize() != GRK_Result::Ok)
    {
        //TODO debug print error fission mailed
        exit(-1);
    }

    //TODO fix my timestep
    time_t currentTime, lastTime;
    time(&lastTime);

    double dt = 0;
    
    //run until break
    while(true)
    {
        m_systemManager.Update(dt);

        time(&currentTime);
        dt = difftime(currentTime, lastTime);
    }
}

GRK_Result GRK_Engine::InjectInitialization(
        std::function<GRK_Result, GRK_EntityComponentManager&> initFunction)
{
    m_initFunction = initFunction;
}

