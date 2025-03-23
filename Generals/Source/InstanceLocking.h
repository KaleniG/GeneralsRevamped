#pragma once

#include <string>

namespace gen
{
  void LockInstance(bool value);
  bool LockInstanceAndCheck(const std::wstring& id);
  void FocusExistingInstanceWindow(const std::wstring& name);
  void UnlockInstance();
}