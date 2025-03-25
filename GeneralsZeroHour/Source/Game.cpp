#include <SAGE.h>

#include <iostream>
#include <chrono>

#include "Core/InstanceLocking.h"
#include "Game.h"

namespace genzh
{
  inline void ParseNoLockInstance(int& argc, char* argv[])
  {
    LockInstance(false);
  }

  void Game::Init(int argc, char* argv[])
  {
    sage::CommandLine::Init();
    sage::CommandLine::AddParameter("-nolockinstance", ParseNoLockInstance);
    sage::CommandLine::ParseArguments(argc, argv);

    if (LockInstanceAndCheck(L"GeneralsZeroHour.lock") == false)
      return;

    sage::FileSystem::Init(std::filesystem::path(argv[0]).parent_path());
    std::cin.get();
    UnlockInstance();
  }

  void Game::Run()
  {
    while (Game::Get().s_IsRunning)
    {
      Timestep timestep = Game::CalculateTimestep();
    }
  }

  Timestep Game::CalculateTimestep()
  {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
    return Timestep(time);
  }
}

int main(int argc, char* argv[])
{
  genzh::Game::Init(argc, argv);
  genzh::Game::Run();
}