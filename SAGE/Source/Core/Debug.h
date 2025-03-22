#pragma once

#include <filesystem>
#include <string>
#include <format>

#include "Core/PlatformDetection.h"

#if defined(PLATFORM_WINDOWS)
#define SAGE_DEBUG_BREAK() __debugbreak()
#else
#include <csignal>
#define SAGE_DEBUG_BREAK() raise(SIGTRAP)
#endif

namespace sage
{
  enum class LogLevel
  {
    Info, Warn, Error, Assert
  };

  void ConsolePrint(const std::string& time, const std::string& message, LogLevel level);
  void SerializeLog(const std::string& time, const std::string& file, const std::string& line, LogLevel level, const std::string& message, bool writeEnd = false);
  void DeserializeLog(const std::filesystem::path& filepath);
  void RuntimePopUp(const std::string& message);
}

#if defined(CONFIG_DEBUG) || defined(CONFIG_RELEASE)
#define SAGE_INFO(fmt, ...) {\
std::string message = std::format(fmt, __VA_ARGS__);\
sage::ConsolePrint(__TIME__, message, sage::LogLevel::Info);\
sage::SerializeLog(__TIME__, __FILE__, std::to_string(__LINE__), sage::LogLevel::Info, message);}\

#define SAGE_WARN(fmt, ...) {\
std::string message = std::format(fmt, __VA_ARGS__);\
sage::ConsolePrint(__TIME__, message, sage::LogLevel::Warn);\
sage::SerializeLog(__TIME__, __FILE__, std::to_string(__LINE__), sage::LogLevel::Warn, message);}\

#define SAGE_ERROR(fmt, ...) {\
std::string message = std::format(fmt, __VA_ARGS__);\
sage::ConsolePrint(__TIME__, message, sage::LogLevel::Error);\
sage::SerializeLog(__TIME__, __FILE__, std::to_string(__LINE__), sage::LogLevel::Error, message);}\

#define SAGE_ASSERT(x, fmt, ...) { if (!(x)) { \
std::string message = std::format(fmt, __VA_ARGS__);\
sage::ConsolePrint(__TIME__, message, sage::LogLevel::Assert);\
sage::SerializeLog( __TIME__, __FILE__, std::to_string(__LINE__), sage::LogLevel::Assert, message, true);\
SAGE_DEBUG_BREAK(); } }\

#elif defined(CONFIG_FINAL)
#define SAGE_INFO(...)

#define SAGE_WARN(...)

#define SAGE_ERROR(fmt, ...) {\
std::string message = std::format(fmt, __VA_ARGS__);\
sage::SerializeLog(__TIME__, __FILE__, std::to_string(__LINE__), sage::LogLevel::Error, message);}\

#define SAGE_ASSERT(x, fmt, ...) { if (!(x)) { \
std::string message = std::format(fmt, __VA_ARGS__);\
sage::SerializeLog(__TIME__, __FILE__, std::to_string(__LINE__), sage::LogLevel::Assert, message);\
sage::RuntimePopUp(message); } }\

#endif