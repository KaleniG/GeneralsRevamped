#include <SAGE.h>

#include <iostream>

#include "InstanceLocking.h"

namespace genzh
{
  void ParseNoLockInstance(int& argc, char* argv[])
  {
    LockInstance(false);
  }

  void main(int argc, char* argv[])
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
}

int main(int argc, char* argv[])
{
  genzh::main(argc, argv);
}