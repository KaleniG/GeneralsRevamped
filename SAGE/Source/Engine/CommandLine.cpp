#include <pch.h>

#include "Engine/CommandLine.h"

namespace sage
{
  void ParseNoLogSerialize()
  {
    DoSerialize(false);
    SAGE_WARN("[SYSTEM] Log serialization is disabled");
  }

  void ParseNoLogPrint()
  {
    DoPrint(false);
  }

  void ParseNoLogPopUp()
  {
    DoPopUp(false);
    SAGE_WARN("[SYSTEM] Log PopUp is disabled");
  }

  void CommandLine::AddParameter(const std::string& arg, const ParseFunc& function)
  {
    CommandLine::Get().s_CommandLineArgsToFuncMap[arg] = function;
  }

  void CommandLine::ParseArguments(int argc, char* argv[])
  {
    auto& commandMap = Get().s_CommandLineArgsToFuncMap;
    commandMap["-nologserialize"] = ParseNoLogSerialize;
    commandMap["-nologprint"] = ParseNoLogPrint;
    commandMap["-nologpopup"] = ParseNoLogPopUp;

    for (int i = 1; i < argc; ++i)
    {
      auto it = commandMap.find(argv[i]);
      if (it != commandMap.end())
        it->second();
      else
        SAGE_ERROR("[USER] Invalid command-line parameter was added: '{}'", argv[i]);
    }
  }
}