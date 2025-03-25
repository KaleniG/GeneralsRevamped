#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>

#include <SAGE.h>

#include "Core/InstanceLocking.h"

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/file.h>
#endif

namespace genzh
{
  static bool s_Lock = true;
  static std::wstring s_UUID;
#ifdef PLATFORM_WINDOWS
  static HANDLE s_MutexHandle = nullptr;
#else
  static int32_t s_LockFileDescriptor = -1;
#endif

  void LockInstance(bool value)
  {
    s_Lock = value;
  }

  bool LockInstanceAndCheck(const std::wstring& id)
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

  void UnlockInstance()
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
