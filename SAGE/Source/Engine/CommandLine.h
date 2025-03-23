#pragma once

#include <unordered_map>
#include <functional>
#include <string>

namespace sage
{
  using ParseFunc = std::function<void()>;
  class CommandLine
  {
  public:
    static void Init();
    static void AddParameter(const std::string& arg, const ParseFunc& function);
    static void ParseArguments(int argc, char* argv[]);

  private:
    static CommandLine& Get() { static CommandLine instance; return instance; }

    CommandLine() = default;
    CommandLine(const CommandLine&) = delete;
    CommandLine& operator=(const CommandLine&) = delete;
    CommandLine(CommandLine&&) = delete;
    CommandLine& operator=(CommandLine&&) = delete;

  private:
    std::unordered_map<std::string, ParseFunc> s_CommandLineArgsToFuncMap;
  };
}