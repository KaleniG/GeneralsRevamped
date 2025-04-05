#include <SAGE.h>

#include <iostream>
#include <chrono>

#include "Core/InstanceLocking.h"
#include "Systems/GameData.h"
#include "Game.h"

namespace genzh
{
  inline void ParseNoLockInstance(int& argc, char* argv[])
  {
    InstanceLock::Enable(false);
  }

  void Game::Init(int argc, char* argv[])
  {
    sage::CommandLine::Init();
    sage::CommandLine::AddParameter("-nolockinstance", ParseNoLockInstance);
    sage::CommandLine::ParseArguments(argc, argv);

    if (InstanceLock::Init(L"GeneralsZeroHour.lock") == false)
      return;

    sage::FileSystem::Init(std::filesystem::path(argv[0]).parent_path());
    
    GameData::Init("Data/INI/Default/GameData.ini", "Data/INI/GameData.ini"
#if defined(CONFIG_DEBUG) || defined(CONFIG_RELEASE)
      , "Data/INI/GameDataDebug.ini"
#endif
    );

    std::cin.get();
  }

  void Game::Run()
  {
    while (Game::Get().s_IsRunning)
    {
      Timestep timestep = Game::CalculateTimestep();
    }
  }

  void Game::End()
  {
    InstanceLock::End();
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
  genzh::Game::End();
}