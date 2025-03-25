#pragma once

#include <SAGE.h>

#include "Core/Timestep.h"

namespace genzh
{
  class Game
  {
  public:
    static void Init(int argc, char* argv[]);
    static void Run();

  private:
    static Timestep CalculateTimestep();

    static Game& Get() { static Game instance; return instance; }

    Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

  private:
    bool s_IsRunning;
  };
}