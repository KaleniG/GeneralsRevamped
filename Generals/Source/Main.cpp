#include <SAGE.h>

#include <iostream>

#include "InstanceLocking.h"

namespace gen
{
  void ParseNoLockInstance()
  {
    LockInstance(false);
  }

  void main(int argc, char* argv[])
  {
    sage::CommandLine::AddParameter("-nolockinstance", ParseNoLockInstance);
    sage::CommandLine::ParseArguments(argc, argv);

    if (LockInstanceAndCheck(L"Generals.lock") == false)
      return;

    UnlockInstance();
  }
}

int main(int argc, char* argv[])
{
  gen::main(argc, argv);
}