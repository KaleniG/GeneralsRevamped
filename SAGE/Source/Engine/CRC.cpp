#include <pch.h>

#include "Engine/FileSystem.h"
#include "Engine/CRC.h"

namespace sage
{
  void CRC::ComputeFile(const std::filesystem::path& filepath)
  {
    RAMFile file = FileSystem::OpenRAMFile(filepath);
    uint8_t crcBlock[65536]; // There is a warning that says that I use too much stack @TODO maybe will fix it after i check if the CRC works with smaller blocks, ah and also, will have to replace the CRC algorithm with a better one, old shit
    int32_t bytesRead = 0;

    while ((bytesRead = file.Read(&crcBlock, 65536)) > 0)
      CRC::Compute(&crcBlock, bytesRead);
  }

  void CRC::Compute(void* buffer, size_t size)
  {
    SAGE_ASSERT(buffer && size > 0, "[SYSTEM] Invalid CRC cmputation block");

    uint8_t* u8ptr = static_cast<uint8_t*>(buffer);

    for (int32_t i = 0; i < size; i++)
    {
      int32_t hiBit;

      if (m_CRC & 0x80000000)
        hiBit = 1;
      else
        hiBit = 0;

      m_CRC <<= 1;
      m_CRC += *(u8ptr++);
      m_CRC += hiBit;
    }
  }
}