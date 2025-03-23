#include <SAGE.h>

#include "InstanceLocking.h"

namespace genzh
{
  void ParseLockInstance()
  {
    if (LockInstanceAndCheck(L"GeneralsZeroHour.lock") == false)
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
  genzh::main(argc, argv);
}