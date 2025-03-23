#pragma once

#include <string>

namespace gen
{
  bool LockInstanceAndCheck(const std::wstring& id);
  void FocusExistingInstanceWindow(const std::wstring& name);
  void UnlockInstance();
}