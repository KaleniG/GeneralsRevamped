#include <SAGE.h>

#include <iostream>

#include "InstanceLocking.h"

namespace genzh
{
  void ParseNoLockInstance()
  {
    LockInstance(false);
  }

  void main(int argc, char* argv[])
  {
    sage::CommandLine::AddParameter("-nolockinstance", ParseNoLockInstance);
    sage::CommandLine::ParseArguments(argc, argv);

    if (LockInstanceAndCheck(L"GeneralsZeroHour.lock") == false)
      return;

    std::cin.get();
    UnlockInstance();
  }
}

int main(int argc, char* argv[])
{
  genzh::main(argc, argv);
}