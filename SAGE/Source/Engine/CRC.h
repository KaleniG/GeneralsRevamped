#pragma once

#include <cstdint>
#include <filesystem>

namespace sage
{
  class CRC
  {
  public:
    CRC() : m_CRC(0) {}

    void ComputeFile(const std::filesystem::path& filepath);

    void Compute(void* buffer, size_t size);
    void Clear() { m_CRC = 0; }
    uint32_t Get() const { return m_CRC; }
  private:
    uint32_t m_CRC;
  };
}