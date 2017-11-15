#include "grok3d_types.h"

#include "Engine.h"
#include "EntityComponentManager.h"
#include "System/SystemManager.h"

#include <chrono>

using namespace std::chrono_literals;

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

void GRK_Engine::Update(double dt)
{
    m_systemManager.UpdateSystems(dt);
}

void GRK_Engine::Render() const
{
    m_systemManager.Render();
}

void GRK_Engine::GarbageCollect()
{
    m_entityComponentManager.GarbageCollect();
}

void GRK_Engine::Run()
{
    using clock = std::chrono::high_resolution_clock;
    using doubleConversion = std::chrono::duration<double>;

    if(Initialize() != GRK_Result::Ok)
    {
        //TODO debug print error fission mailed
        exit(-1);
    }
    
    //TODO use CVAR to set this as tickrate
    //this is 144hz period in ns: 6944444ns
    const std::chrono::nanoseconds dt(6944444ns);

    //fix my timestep referneced here: https://gafferongames.com/post/fix_your_timestep/
    auto currentTime = clock::now();

    //t is total simulation time
    std::chrono::nanoseconds t(0s);
    std::chrono::nanoseconds accumulator(0s);
    
    //run until break
    while(true)
    {
        auto newTime = clock::now();
        auto prevFrameTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += prevFrameTime;

        while (accumulator >= dt)
        {
            this->Update(doubleConversion(dt).count());
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

template<>
Grok3d::Entities::GRK_Entity Grok3d::GRK_EntityComponentManager::s_NextEntityId = 1;

