#pragma once

#define USE_LEGACY_DATA_DIRECTORY //Temporary

#include <string>
#include <filesystem>

namespace sage
{
  namespace InstanceLock
  {
    void Enable(bool value);
    bool Init(const std::wstring& id);
    void FocusExistingInstanceWindow(const std::wstring& name);
    void End();
  }

  namespace User
  {
    std::filesystem::path GetConfigCrossPlatformDirectory(const std::string& directory_name);
  }
}