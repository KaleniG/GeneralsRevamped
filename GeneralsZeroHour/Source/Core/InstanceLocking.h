#pragma once

#include <string>

namespace genzh
{
  void LockInstance(bool value);
  bool LockInstanceAndCheck(const std::wstring& id);
  void FocusExistingInstanceWindow(const std::wstring& name);
  void UnlockInstance();
}