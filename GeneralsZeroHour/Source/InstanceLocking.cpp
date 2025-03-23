#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>

#include <SAGE.h>

#include "InstanceLocking.h"

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
  static std::wstring uuid;
#ifdef PLATFORM_WINDOWS
  static HANDLE mutexHandle = nullptr;
#else
  static int32_t lockFileDescriptor = -1;
#endif

  bool LockInstanceAndCheck(const std::wstring& id)
  {
    if (!uuid.empty())
    {
      SAGE_ERROR("[SYSTEM] The instance of this program has already been locked");
      return false;
    }

#ifdef PLATFORM_WINDOWS
    mutexHandle = CreateMutexW(nullptr, TRUE, id.c_str());
    if (!mutexHandle)
    {
      SAGE_ERROR("[SYSTEM] Failed to create mutex! Error code: {}", GetLastError());
      return false;
    }
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
      SAGE_ERROR("[SYSTEM] Another instance is already running!");
      CloseHandle(mutexHandle);
      return false;
    }
#else
    std::string uuidStr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(id);

    lockFileDescriptor = open(uuidStr.c_str(), O_CREAT | O_RDWR, 0666);
    if (lockFileDescriptor < 0)
    {
      SAGE_ERROR("[SYSTEM] Failed to open the lock file: {}", uuidStr);
      return false;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if (fcntl(lockFileDescriptor, F_SETLK, &lock) == -1)
    {
      SAGE_ERROR("[SYSTEM] Another instance is already running!");
      close(lockFileDescriptor);
      return false;
    }
#endif

    uuid = id;
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
    if (uuid.empty()) return;

    std::string uuidStr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(uuid);

#ifdef PLATFORM_WINDOWS
    if (mutexHandle)
    {
      ReleaseMutex(mutexHandle);
      CloseHandle(mutexHandle);
      mutexHandle = nullptr;
    }
#else
    if (lockFileDescriptor >= 0)
    {
      struct flock lock;
      lock.l_type = F_UNLCK;
      lock.l_whence = SEEK_SET;
      lock.l_start = 0;
      lock.l_len = 0;

      fcntl(lockFileDescriptor, F_SETLK, &lock);
      close(lockFileDescriptor);
      lockFileDescriptor = -1;
    }

    unlink(uuidStr.c_str());
#endif
    uuid.clear();
  }
}
