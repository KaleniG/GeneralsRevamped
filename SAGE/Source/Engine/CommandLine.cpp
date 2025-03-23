#include <pch.h>

#include "Engine/CommandLine.h"

namespace sage
{
  void CommandLine::AddParameter(const std::string& arg, const ParseFunc& function)
  {
    CommandLine::Get().s_CommandLineArgsToFuncMap[arg] = function;
  }

  void CommandLine::ParseArguments(int argc, char* argv[])
  {
    for (int i = 1; i < argc; ++i)
    {
      auto& commandMap = Get().s_CommandLineArgsToFuncMap;
      auto it = commandMap.find(argv[i]);

      if (it != commandMap.end())
        it->second();
    }
  }
}