/* Copyright (c) 2017 Brandon Pollack
* Contact @ grok3dengine@gmail.com
* This file is available under the MIT license included in the project
*/
#include "grok3d_types.h"

#include "Engine.h"

#include <chrono>

using namespace std::chrono_literals;

using namespace Grok3d;
using namespace Grok3d::Systems;

GRK_Engine::GRK_Engine() noexcept {
  //Inject dependency references so we can update the systems from ECM and set up systems with ECM
  m_entityComponentManager.Initialize(&m_systemManager);
  m_systemManager.Initialize(&m_entityComponentManager);
}

GRK_Engine::GRK_Engine(std::function<GRK_Result(GRK_EntityComponentManager &)> initFunction) noexcept : GRK_Engine() {
  m_initFunction = initFunction;
}

auto GRK_Engine::Initialize() -> GRK_Result {
  if (m_initFunction) {
    return m_initFunction(m_entityComponentManager);
  } else {
    return GRK_Result::EngineFailureNoInitialState;
  }
}

auto GRK_Engine::Update(double dt) -> void {
  m_systemManager.UpdateSystems(dt);
}

auto GRK_Engine::Render() const -> GRK_Result {
  return m_systemManager.Render();
}

auto GRK_Engine::GarbageCollect() -> void {
  m_entityComponentManager.GarbageCollect();
}

auto GRK_Engine::Run() -> void {
  using clock = std::chrono::high_resolution_clock;
  using doubleConversion = std::chrono::duration<double>;

  if (Initialize() != GRK_Result::Ok) {
    //TODO debug print error fission mailed
    exit(-1);
  }

  //TODO use CVAR to set this as tickrate
  //this is 144hz period in ns: 6944444ns
  const auto dt = std::chrono::nanoseconds(6944444ns);

  //fix my timestep referneced here: https://gafferongames.com/post/fix_your_timestep/
  auto currentTime = clock::now();

  //t is total simulation time
  auto t = std::chrono::nanoseconds(0s);
  auto accumulator = std::chrono::nanoseconds(0s);

  //run until break
  while (true) {
    auto newTime = clock::now();
    auto prevFrameTime = newTime - currentTime;
    currentTime = newTime;

    accumulator += prevFrameTime;

    while (accumulator >= dt) {
      this->Update(doubleConversion(dt).count());
      accumulator -= dt;
      t += dt;
    }

    //TODO change this, shouldnt exit game engine should just not render until GLFW reinits
    //rendering terminated so we should exit the engine
    if (this->Render() == GRK_Result::RenderingTerminated) {
      break;
    }

    this->GarbageCollect();
  }
}

auto GRK_Engine::InjectInitialization(
  std::function<GRK_Result(GRK_EntityComponentManager &)> initFunction) -> GRK_Result {
  m_initFunction = initFunction;

  return GRK_Result::Ok;
}
