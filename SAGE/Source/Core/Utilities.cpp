#include <pch.h>

#if defined(PLATFORM_WINDOWS)
#include <winsock2.h>
#include <windows.h>
#include <shlobj.h>
#elif defined(PLATFORM_MACOS) || defined(PLATFORM_LINUX)
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>
#endif

#include <asio.hpp>

#include "Core/Utilities.h"

namespace sage
{
  namespace InstanceLock
  {
    static bool s_Lock = true;
    static std::wstring s_UUID;
#ifdef PLATFORM_WINDOWS
    static HANDLE s_MutexHandle = nullptr;
#else
    static int32_t s_LockFileDescriptor = -1;
#endif

    void Enable(bool value)
    {
      s_Lock = value;
    }

    bool Init(const std::wstring& id)
    {
      if (!s_Lock)
      {
        SAGE_WARN("[SYSTEM] This instance of the applications is lock-free");
        return false;
      }

      if (!s_UUID.empty())
      {
        SAGE_ERROR("[SYSTEM] The instance of this program has already been locked");
        return false;
      }

#ifdef PLATFORM_WINDOWS
      s_MutexHandle = CreateMutexW(nullptr, TRUE, id.c_str());
      if (!s_MutexHandle)
      {
        SAGE_ERROR("[SYSTEM] Failed to create mutex! Error code: {}", GetLastError());
        return false;
      }
      if (GetLastError() == ERROR_ALREADY_EXISTS)
      {
        SAGE_ERROR("[SYSTEM] Another instance is already running!");
        CloseHandle(s_MutexHandle);
        return false;
      }
#else
      std::string uuidStr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(id);

      s_LockFileDescriptor = open(uuidStr.c_str(), O_CREAT | O_RDWR, 0666);
      if (s_LockFileDescriptor < 0)
      {
        SAGE_ERROR("[SYSTEM] Failed to open the lock file: {}", uuidStr);
        return false;
      }

      struct flock lock;
      lock.l_type = F_WRLCK;
      lock.l_whence = SEEK_SET;
      lock.l_start = 0;
      lock.l_len = 0;

      if (fcntl(s_LockFileDescriptor, F_SETLK, &lock) == -1)
      {
        SAGE_ERROR("[SYSTEM] Another instance is already running!");
        close(s_LockFileDescriptor);
        return false;
      }
#endif

      s_UUID = id;
      return true;
    }

    void FocusExistingInstanceWindow(const std::wstring& window_name)
    {
#ifdef PLATFORM_WINDOWS
      HWND hwnd = FindWindowW(nullptr, window_name.c_str());
      if (hwnd)
      {
        SetForegroundWindow(hwnd);
        ShowWindow(hwnd, SW_RESTORE);
      }
#elif PLATFORM_APPLE
      std::string appleCmd = "osascript -e 'tell application \"System Events\" to set frontmost of process \"";
      appleCmd += std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(window_name);
      appleCmd += "\" to true'";
      system(appleCmd.c_str());
#elif PLATFORM_LINUX
      std::string linuxCmd = "wmctrl -a '";
      linuxCmd += std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(window_name);
      linuxCmd += "'";
      system(linuxCmd.c_str());
#endif
    }

    void End()
    {
      if (s_UUID.empty()) return;

      std::string uuidStr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(s_UUID);

#ifdef PLATFORM_WINDOWS
      if (s_MutexHandle)
      {
        ReleaseMutex(s_MutexHandle);
        CloseHandle(s_MutexHandle);
        s_MutexHandle = nullptr;
      }
#else
      if (s_LockFileDescriptor >= 0)
      {
        struct flock lock;
        lock.l_type = F_UNLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        fcntl(s_LockFileDescriptor, F_SETLK, &lock);
        close(s_LockFileDescriptor);
        s_LockFileDescriptor = -1;
      }

      unlink(uuidStr.c_str());
#endif
      s_UUID.clear();
    }
  }

  namespace User
  {
    std::filesystem::path GetUserDataDirectory(const std::string& directory_name)
    {
      std::filesystem::path directory;

#if defined(PLATFORM_WINDOWS)

#ifdef USE_LEGACY_DATA_DIRECTORY

      std::wstring documentsDir;
      PWSTR pathPWSTR = nullptr;
      HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &pathPWSTR);
      if (SUCCEEDED(hr))
      {
        documentsDir = std::wstring(pathPWSTR);
        CoTaskMemFree(pathPWSTR);
      }
      else
      {
        wchar_t pathWCHAR[_MAX_PATH];
        if (SHGetSpecialFolderPathW(NULL, pathWCHAR, CSIDL_MYDOCUMENTS, true))
          documentsDir = std::wstring(pathWCHAR);
        else
        {
          HRESULT hr = SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, 0, pathWCHAR);
          if (SUCCEEDED(hr))
            documentsDir = std::wstring(pathWCHAR);
        }
      }
      if (!documentsDir.empty())
        directory = std::filesystem::path(documentsDir) / directory_name;

#else

      std::wstring appDataDir;
      const char* appData = std::getenv("LOCALAPPDATA");
      if (appData)
      {
        int32_t sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, appData, -1, nullptr, 0);
        appDataDir.resize(sizeNeeded);
        MultiByteToWideChar(CP_UTF8, 0, appData, -1, &appDataDir[0], sizeNeeded);
      }
      else
      {
        PWSTR pathPWSTR = nullptr;
        HRESULT hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, nullptr, &pathPWSTR);
        if (SUCCEEDED(hr))
        {
          appDataDir = std::wstring(pathPWSTR);
          CoTaskMemFree(pathPWSTR);
        }
        else
        {
          wchar_t pathWCHAR[_MAX_PATH];
          if (SHGetSpecialFolderPathW(NULL, pathWCHAR, CSIDL_LOCAL_APPDATA, true))
            appDataDir = std::wstring(pathWCHAR);
          else
          {
            HRESULT hr = SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, pathWCHAR);
            if (SUCCEEDED(hr))
              appDataDir = std::wstring(pathWCHAR);
          }
        }
      }

      if (!appDataDir.empty())
        directory = std::filesystem::path(appData) / directory_name;

#endif

#elif defined(PLATFORM_MACOS)

      struct passwd pwd;
      char buffer[1024];
      struct passwd* result = nullptr;

      const char* homeDir = std::getenv("HOME");
      if (!homeDir)
      {
        if (getpwuid_r(getuid(), &pwd, buffer, sizeof(buffer), &result) == 0 && result != nullptr)
        {
          homeDir = pwd.pw_dir;
        }
        else
        {
          struct passwd* pwdResult = getpwuid(getuid());
          if (pwdResult)
            homeDir = pwdResult->pw_dir;
        }
      }

      if (homeDir)
        directory = std::filesystem::path(homeDir) / "Library" / "Application Support" / directory_name;

#elif defined(PLATFORM_LINUX)

      std::filesystem::path configPath;
      const char* path = std::getenv("XDG_CONFIG_HOME");
      if (path)
        configPath = std::filesystem::path(path);
      else
      {
        path = std::getenv("HOME");
        if (path)
          configPath = std::filesystem::path(path) / ".config";
        else
        {
          struct passwd pwd;
          char buffer[1024];
          struct passwd* result = nullptr;

          if (getpwuid_r(getuid(), &pwd, buffer, sizeof(buffer), &result) == 0 && result != nullptr)
          {
            path = pwd.pw_dir;
            configPath = std::filesystem::path(path) / ".config";
          }
          else
          {
            struct passwd* pwdResult = getpwuid(getuid());
            if (pwdResult)
            {
              path = pwdResult->pw_dir;
              configPath = std::filesystem::path(path) / ".config";
            }
          }
        }
      }

      if (!configPath.empty())
        directory = configPath / directory_name;

#endif

      SAGE_ASSERT(!directory.empty(), "[SYSTEM] Couldn't get the user data directory");

      if (!std::filesystem::exists(directory))
        std::filesystem::create_directories(directory);
      return directory;
    }

    std::string GetLocalHostName()
    {
      return asio::ip::host_name();
    }

    std::vector<asio::ip::address_v4> GetLocalIPAddresses()
    {
      std::vector<asio::ip::address_v4> localIPs;
      try
      {
        asio::io_context io_context; // Momentarely here but later should be located elsewhere, like in a networking class @TODO
        std::string hostname = asio::ip::host_name();
        asio::ip::tcp::resolver resolver(io_context); // same here
        asio::ip::tcp::resolver::results_type results = resolver.resolve(hostname, "");

        for (const auto& entry : results)
        {
          asio::ip::address addr = entry.endpoint().address();
          if (addr.is_v4())
            localIPs.push_back(addr.to_v4());
        }
      }
      catch (const std::exception& e)
      {
        SAGE_ASSERT(false, "[SYSTEM] Caught an exception while trying to retrieve the local IPv4 addresses of the local host. Exception: '{}'", e.what());
      }
      
      SAGE_ASSERT(!localIPs.empty(), "[SYSTEM] Failed to retrieve the local IP addresses");
      return localIPs;
    }
  }
}