#include <pch.h>

#include <nlohmann/json.hpp>

#include "Core/Debug.h"

#if defined(PLATFORM_WINDOWS)
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace sage
{
#if defined(PLATFORM_WINDOWS)
  static HANDLE s_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

#if defined(CONFIG_DEBUG)
#define SERIALIZE_LOG_FILENAME "Debug - " __DATE__ ".log"
#elif defined(CONFIG_RELEASE)
#define SERIALIZE_LOG_FILENAME "Release - " __DATE__ ".log"
#elif defined(CONFIG_FINAL)
#define SERIALIZE_LOG_FILENAME "Final - " __DATE__ ".log"
#endif

  class OnDestructWrapper
  {
  public:
    ~OnDestructWrapper()
    {
      if (OnDestructWrapper::Get().s_Used)
        OnDestructWrapper::Get().s_LogFileStream << "[LOG-END]" << std::endl;
    }

    static std::ofstream& GetLogFile() { return OnDestructWrapper::Get().s_LogFileStream; }
  private:
    static OnDestructWrapper& Get() { static OnDestructWrapper instance; instance.s_Used = true; return instance; }

    OnDestructWrapper()
      : s_LogFileStream(SERIALIZE_LOG_FILENAME, std::ios::app), s_Used(false) {}
    OnDestructWrapper(const OnDestructWrapper&) = delete;
    OnDestructWrapper& operator=(const OnDestructWrapper&) = delete;
    OnDestructWrapper(OnDestructWrapper&&) = delete;
    OnDestructWrapper& operator=(OnDestructWrapper&&) = delete;

  private:
    std::ofstream s_LogFileStream;
    bool s_Used;
  };

  void ConsolePrint(const std::string& time, const std::string& message, LogLevel level)
  {
    std::string finalMessage;
#if !defined(PLATFORM_WINDOWS)
    switch (level)
    {
    case LogLevel::Info:
      finalMessage.append("\033[38;5;15m\033[48;5;0m");
      break;
    case LogLevel::Warn:
      finalMessage.append("\033[38;5;11m\033[48;5;0m");
      break;
    case LogLevel::Error:
      finalMessage.append("\033[38;5;9m\033[48;5;0m");
      break;
    case LogLevel::Assert:
      finalMessage.append("\033[38;5;15m\033[48;5;9m");
      break;
    }
#else
    switch (level)
    {
    case LogLevel::Info:
      SetConsoleTextAttribute(s_ConsoleHandle, 15 | (0 << 4));
      break;
    case LogLevel::Warn:
      SetConsoleTextAttribute(s_ConsoleHandle, 14 | (0 << 4));
      break;
    case LogLevel::Error:
      SetConsoleTextAttribute(s_ConsoleHandle, 12 | (0 << 4));
      break;
    case LogLevel::Assert:
      SetConsoleTextAttribute(s_ConsoleHandle, 15 | (12 << 4));
      break;
    }
#endif
    finalMessage.append("[" + time + "]");
    switch (level)
    {
    case LogLevel::Info:
      finalMessage.append("[INFO]");
      break;
    case LogLevel::Warn:
      finalMessage.append("[WARN]");
      break;
    case LogLevel::Error:
      finalMessage.append("[ERROR]");
      break;
    case LogLevel::Assert:
      finalMessage.append("[ASSERT]");
      break;
    }
    finalMessage.append(message);
#if !defined(PLATFORM_WINDOWS)
    finalMessage.append("\033[0m");
#endif
    finalMessage.append("\n");
#if defined(PLATFORM_WINDOWS)
    DWORD written;
    WriteConsoleA(s_ConsoleHandle, finalMessage.c_str(), finalMessage.size(), &written, nullptr);
    SetConsoleTextAttribute(s_ConsoleHandle, 7 | (0 << 4));
#else
    write(STDOUT_FILENO, finalMessage.c_str(), finalMessage.size());
#endif
  }

  void SerializeLog(const std::string& time, const std::string& file, const std::string& line, LogLevel level, const std::string& message, bool writeEnd)
  {
    if (!OnDestructWrapper::GetLogFile().is_open())
      return;

    nlohmann::json logEntry = 
    {
        {"time", time},
        {"file", file},
        {"line", line},
        {"level", static_cast<int32_t>(level)},
        {"message", message}
    };

    OnDestructWrapper::GetLogFile() << logEntry.dump() << std::endl;
    if (writeEnd)
      OnDestructWrapper::GetLogFile() << "[LOG-END]" << std::endl;
  }

  void DeserializeLog(const std::filesystem::path& filepath)
  {
    std::ifstream logFile(filepath);
    if (!logFile.is_open())
      return;

    std::string line;
    while (std::getline(logFile, line))
    {
      if (line == "[LOG-END]")
      {
#if defined(PLATFORM_WINDOWS)
        SetConsoleTextAttribute(s_ConsoleHandle, 7 | (0 << 4));
        DWORD written;
        std::string endLog = "*****************************************************************************\n-----------------------------------------------------------------------------\n*****************************************************************************\n";
        WriteConsoleA(s_ConsoleHandle, endLog.c_str(), endLog.size(), &written, nullptr);
#else
        write(STDOUT_FILENO, endLog.c_str(), endLog.size());
#endif
        continue;
      }

      try
      {
        nlohmann::json logEntry = nlohmann::json::parse(line);
        std::string finalMessage;
        LogLevel level = static_cast<LogLevel>(logEntry["level"].get<int32_t>());
#if !defined(PLATFORM_WINDOWS)
        switch (level)
        {
        case LogLevel::Info:
          finalMessage.append("\033[38;5;15m\033[48;5;0m");
          break;
        case LogLevel::Warn:
          finalMessage.append("\033[38;5;11m\033[48;5;0m");
          break;
        case LogLevel::Error:
          finalMessage.append("\033[38;5;9m\033[48;5;0m");
          break;
        case LogLevel::Assert:
          finalMessage.append("\033[38;5;15m\033[48;5;9m");
          break;
        }
#else
        switch (level)
        {
        case LogLevel::Info:
          SetConsoleTextAttribute(s_ConsoleHandle, 15 | (0 << 4));
          break;
        case LogLevel::Warn:
          SetConsoleTextAttribute(s_ConsoleHandle, 14 | (0 << 4));
          break;
        case LogLevel::Error:
          SetConsoleTextAttribute(s_ConsoleHandle, 12 | (0 << 4));
          break;
        case LogLevel::Assert:
          SetConsoleTextAttribute(s_ConsoleHandle, 15 | (12 << 4));
          break;
        }
#endif
        std::string time = logEntry["time"];
        std::string file = logEntry["file"];
        std::string line = logEntry["line"];
        finalMessage.append("[" + time + "]");
        finalMessage.append("[" + file + "]");
        finalMessage.append("[Line:" + line + "]");
        switch (level)
        {
        case LogLevel::Info:
          finalMessage.append("[INFO]");
          break;
        case LogLevel::Warn:
          finalMessage.append("[WARN]");
          break;
        case LogLevel::Error:
          finalMessage.append("[ERROR]");
          break;
        case LogLevel::Assert:
          finalMessage.append("[ASSERT]");
          break;
        }
        std::string message = logEntry["message"];
        finalMessage.append(message);
#if !defined(PLATFORM_WINDOWS)
        finalMessage.append("\033[0m");
#endif
        finalMessage.append("\n");
#if defined(PLATFORM_WINDOWS)
        DWORD written;
        WriteConsoleA(s_ConsoleHandle, finalMessage.c_str(), finalMessage.size(), &written, nullptr);
        SetConsoleTextAttribute(s_ConsoleHandle, 7 | (0 << 4));
#else
        write(STDOUT_FILENO, finalMessage.c_str(), finalMessage.size());
#endif
      }
      catch (const std::exception& e)
      {
        SAGE_ERROR("[SYSTEM] Failed to parse json '{}': {}", filepath.string(), e.what());
      }
    }
  }

  void RuntimePopUp(const std::string& message)
  {
#if defined(PLATFORM_WINDOWS)
    MessageBoxA(NULL, message.c_str(), "Error", MB_OK | MB_ICONERROR);
#elif (PLATFORM_APPLE)
    std::string cmd = "osascript -e 'display alert \"Error\" message \"" + message + "\"'";
    system(cmd.c_str());
#endif
  }
}