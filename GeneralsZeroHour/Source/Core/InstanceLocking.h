#pragma once

#include <string>

namespace genzh
{
  namespace InstanceLock
  {
    void Enable(bool value);
    bool Init(const std::wstring& id);
    void FocusExistingInstanceWindow(const std::wstring& name);
    void End();
  }
}