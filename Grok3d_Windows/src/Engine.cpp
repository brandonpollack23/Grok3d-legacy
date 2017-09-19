#include "Engine.h"
#include "entityComponentManager.h"
#include "System/SystemManager.h"

#include <ctime>

using namespace Grok3d;
using namespace Grok3d::Systems;

GRK_Engine::GRK_Engine()
{
    //Inject systemManager dependency so we can update the systems
    m_entityComponentManager.Initialize(&m_systemManager);
}

GRK_Engine::GRK_Engine(std::function<GRK_Result(GRK_EntityComponentManager&)> initFunction) : GRK_Engine()
{
    m_initFunction = initFunction;    
}

GRK_Result GRK_Engine::Initialize()
{
    if(m_initFunction)
    {
        return m_initFunction(m_entityComponentManager);
    }
    else
    {
        return GRK_Result::EngineFailureNoInitialState;
    }
}

void GRK_Engine::Update(float dt)
{
    m_systemManager.UpdateSystems(dt);
}

void GRK_Engine::Render()
{
    m_systemManager.Render();
}

void GRK_Engine::GarbageCollect()
{
    m_entityComponentManager.GarbageCollect();
}

void GRK_Engine::Run()
{
    if(Initialize() != GRK_Result::Ok)
    {
        //TODO debug print error fission mailed
        exit(-1);
    }
    
    //TODO use CVAR to set this as tickrate
    const time_t dt = 1 / 60.0; 

    //fix my timestep referneced here: https://gafferongames.com/post/fix_your_timestep/
    time_t currentTime, newTime;
    time(&currentTime);

    //t is total simulation time
    time_t t = 0.0;
    time_t accumulator = 0;
    
    //run until break
    while(true)
    {
        time(&newTime);
        time_t prevFrameTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += prevFrameTime;

        while (accumulator >= dt)
        {
            this->Update(dt);
            accumulator -= dt;
            t += dt;
        }

        this->Render();
        
        this->GarbageCollect();
    }
}

GRK_Result GRK_Engine::InjectInitialization(
        std::function<GRK_Result(GRK_EntityComponentManager&)> initFunction)
{
    m_initFunction = initFunction;

    return GRK_Result::Ok;
}

