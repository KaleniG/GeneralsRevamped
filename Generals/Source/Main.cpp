#include <SAGE.h>

#include "InstanceLocking.h"

namespace gen
{
  void ParseLockInstance()
  {
    if (LockInstanceAndCheck(L"Generals.lock") == false)
      SAGE_ERROR("Failed to lock the application instance");
  }

  void main(int argc, char* argv[])
  {
    sage::CommandLine::AddArgumentBinding("-lockinstance", ParseLockInstance);


    UnlockInstance();
  }
}

int main(int argc, char* argv[])
{
  gen::main(argc, argv);
}