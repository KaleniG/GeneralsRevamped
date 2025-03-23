#include <pch.h>

#include "Core/File.h"

namespace sage
{
  DiskFile::DiskFile(const std::filesystem::path& filepath, std::ios_base::openmode access)
    : m_Filepath(filepath), m_Access(access), m_Filestream(filepath, access)
  {
    SAGE_ASSERT(std::filesystem::exists(m_Filepath) && !m_Filepath.empty(), "[SYSTEM] Invalid filepath '{}'", filepath.string());
    SAGE_ASSERT(m_Filestream.good() && m_Filestream.is_open(), "[SYSTEM] Failed to open the file '{}'", filepath.string());
  }

  int32_t DiskFile::Read(void* buffer, int32_t bytes)
  {
    SAGE_ASSERT(m_Filestream.good(), "[SYSTEM] The file is not good");

    m_Filestream.read(reinterpret_cast<char*>(buffer), bytes);
    SAGE_ASSERT(!m_Filestream.fail(), "[SYSTEM] Failed to read from the file '{}'", m_Filepath.string());

    std::streamsize count = m_Filestream.gcount();
    return static_cast<int32_t>(count);
  }

  void DiskFile::Write(const void* buffer, int32_t bytes)
  {
    SAGE_ASSERT(m_Filestream.good(), "[SYSTEM] The file is not good");

    m_Filestream.write(reinterpret_cast<const char*>(buffer), bytes);
    SAGE_ASSERT(!m_Filestream.fail(), "[SYSTEM] Failed to write into the file '{}'", m_Filepath.string());
  }

  int32_t DiskFile::Seek(int32_t position, SeekMode mode)
  {
    SAGE_ASSERT(m_Filestream.good(), "[SYSTEM] The file is not good");

    std::ios_base::seekdir dir;
    switch (mode)
    {
    case SeekMode::Start:
      dir = std::ios::beg;
      break;
    case SeekMode::Current:
      dir = std::ios::cur;
      break;
    case SeekMode::End:
      dir = std::ios::end;
      break;
    }

    m_Filestream.clear();
    m_Filestream.seekg(position, dir);
    m_Filestream.seekp(position, dir);

    SAGE_ASSERT(!m_Filestream.fail(), "[SYSTEM] Failed to perform the operation");

    std::streampos pos = (m_Access & std::ios::in) ? m_Filestream.tellg() : m_Filestream.tellp();
    return static_cast<int32_t>(pos);
  }

  int32_t DiskFile::GetSize()
  {
    int32_t currentPos = DiskFile::GetPosition();
    std::streampos fileSize;
    if (m_Access & std::ios::in)
    {
      m_Filestream.seekg(0, std::ios::end);
      fileSize = m_Filestream.tellg();
      m_Filestream.seekg(currentPos);
    }
    else
    {
      m_Filestream.seekp(0, std::ios::end);
      fileSize = m_Filestream.tellp();
      m_Filestream.seekp(currentPos);
    }
    return fileSize;
  }

  int32_t DiskFile::GetPosition()
  {
    SAGE_ASSERT(m_Filestream.good(), "[SYSTEM] The file is not good");

    std::streampos currentPos;

    if (m_Access & std::ios::in)
      currentPos = m_Filestream.tellg();
    else
      currentPos = m_Filestream.tellp();

    return static_cast<int32_t>(currentPos);
  }

  bool DiskFile::IsEndOfFile()
  {
    int32_t currentPos = DiskFile::GetPosition();
    std::streampos fileSize;
    if (m_Access & std::ios::in)
    {
      m_Filestream.seekg(0, std::ios::end);
      fileSize = m_Filestream.tellg();
      m_Filestream.seekg(currentPos);
    }
    else
    {
      m_Filestream.seekp(0, std::ios::end);
      fileSize = m_Filestream.tellp();
      m_Filestream.seekp(currentPos);
    }
    return currentPos == fileSize;
  }

  const std::filesystem::path& DiskFile::GetFilepath() const
  {
    SAGE_ASSERT(m_Filestream.good(), "[SYSTEM] The file is not good");
    return m_Filepath;
  }

  bool DiskFile::GetLine(std::string& line)
  {
    if (std::getline(m_Filestream, line))
      return true;
    return false;
  }

  RAMFile::RAMFile(const std::filesystem::path& filepath)
    : m_Filepath(filepath), m_Position(0)
  {
    std::ifstream file(filepath, std::ios::binary);
    SAGE_ASSERT(file.good() && file.is_open(), "[SYSTEM] Failed to open the file '{}'", filepath.string());

    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    m_Data.resize(static_cast<size_t>(fileSize) + 1);
    file.read(m_Data.data(), fileSize);

    SAGE_ASSERT(!file.fail(), "[SYSTEM] Failed to read the file '{}'", filepath.string());

    m_Data[fileSize] = '\0';
  }

  int32_t RAMFile::Read(void* buffer, int32_t bytes)
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");
    int32_t bytesLeft = static_cast<int32_t>(m_Data.size()) - m_Position;
    if (static_cast<int32_t>(bytes) > bytesLeft)
      bytes = bytesLeft;
    if (bytes > 0 && buffer != nullptr)
      std::memcpy(buffer, m_Data.data() + m_Position, bytes);
    m_Position += static_cast<int32_t>(bytes);
    return static_cast<int32_t>(bytes);
  }

  int32_t RAMFile::Seek(int32_t position, SeekMode mode)
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");
    int32_t newPos = 0;
    switch (mode)
    {
    case SeekMode::Start:
      newPos = static_cast<int32_t>(position);
      break;
    case SeekMode::Current:
      newPos = m_Position + static_cast<int32_t>(position);
      break;
    case SeekMode::End:
      newPos = static_cast<int32_t>(m_Data.size()) + static_cast<int32_t>(position);
      break;
    }

    SAGE_ASSERT(newPos < m_Data.size() && newPos > 0, "[SYSTEM] Can't seek a position out of bounds");

    m_Position = newPos;
    return m_Position;
  }

  int32_t RAMFile::GetSize() const
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");
    return m_Data.size();
  }

  int32_t RAMFile::GetPosition() const
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");
    return m_Position;
  }

  bool RAMFile::IsEndOfFile() const
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");
    return m_Position == m_Data.size();
  }

  const std::filesystem::path& RAMFile::GetFilepath() const
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");
    return m_Filepath;
  }
}