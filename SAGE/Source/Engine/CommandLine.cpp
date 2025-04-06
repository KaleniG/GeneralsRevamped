#include <pch.h>

#include "Core/Utilities.h"
#include "Engine/CommandLine.h"

namespace sage
{
  inline void ParseNoLogSerialize(int& argc, char* argv[])
  {
    DoSerialize(false);
    SAGE_WARN("[SYSTEM] Log serialization is disabled");
  }

  inline void ParseNoLogPrint(int& argc, char* argv[])
  {
    DoPrint(false);
  }

  inline void ParseNoLogPopUp(int& argc, char* argv[])
  {
    DoPopUp(false);
    SAGE_WARN("[SYSTEM] Log PopUp is disabled");
  }

  inline void ParseNoLockInstance(int& argc, char* argv[])
  {
    InstanceLock::Enable(false);
    SAGE_WARN("[SYSTEM] Instance-locking has been disabled");
  }

  void CommandLine::Init()
  {
    auto& commandMap = CommandLine::Get().s_CommandLineArgsToFuncMap;
    commandMap["-nologprint"] = ParseNoLogPrint;
    commandMap["-nologserialize"] = ParseNoLogSerialize;
    commandMap["-nologpopup"] = ParseNoLogPopUp;
    commandMap["-nolockinstance"] = ParseNoLockInstance;
  }

  void CommandLine::AddParameter(const std::string& arg, const ParseFunc& function)
  {
    CommandLine::Get().s_CommandLineArgsToFuncMap[arg] = function;
  }

  void CommandLine::ParseArguments(int argc, char* argv[])
  {
    CommandLine::Get().s_ExecutableFilepath = std::filesystem::path(argv[0]);
    auto& commandMap = CommandLine::Get().s_CommandLineArgsToFuncMap;
    for (int i = 1; i < argc; ++i)
    {
      auto it = commandMap.find(argv[i]);
      if (it != commandMap.end())
        it->second(i, argv);
      else
        SAGE_ERROR("[USER] Invalid command-line parameter was added: '{}'", argv[i]);
    }
  }
}