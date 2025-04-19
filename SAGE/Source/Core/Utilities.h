#pragma once

#define USE_LEGACY_DATA_DIRECTORY //Temporary

#include <filesystem>
#include <cstdint>
#include <string>
#include <vector>

#include <asio.hpp>

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
    std::filesystem::path GetUserDataDirectory(const std::string& directory_name);
    std::string GetLocalHostName();
    std::vector<asio::ip::address_v4> GetLocalIPAddresses();
  }
}