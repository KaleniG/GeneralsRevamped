#include <pch.h>

#include "Core/File.h"

namespace sage
{
  SAGEFile::SAGEFile(const std::filesystem::path& filepath)
    : m_Filepath(filepath) {}

  DiskFile::DiskFile()
    : SAGEFile() {}

  DiskFile::DiskFile(const std::filesystem::path& filepath, std::ios_base::openmode access)
    : SAGEFile(filepath), m_Filestream(filepath, access), m_Access(access)
  {
    SAGE_ASSERT(std::filesystem::exists(DiskFile::GetFilepath()) && !DiskFile::GetFilepath().empty(), "[SYSTEM] Invalid filepath '{}'", filepath.string());
    SAGE_ASSERT(m_Filestream.good() && m_Filestream.is_open(), "[SYSTEM] Failed to open the file '{}'", filepath.string());
  }

  int32_t DiskFile::Read(void* buffer, int32_t bytes)
  {
    SAGE_ASSERT(m_Filestream.good(), "[SYSTEM] The file is not good");

    m_Filestream.read(reinterpret_cast<char*>(buffer), bytes);
    SAGE_ASSERT(!m_Filestream.fail(), "[SYSTEM] Failed to read from the file '{}'", SAGEFile::GetFilepath().string());

    std::streamsize count = m_Filestream.gcount();
    return static_cast<int32_t>(count);
  }

  void DiskFile::Write(const void* buffer, int32_t bytes)
  {
    SAGE_ASSERT(m_Filestream.good(), "[SYSTEM] The file is not good");

    m_Filestream.write(reinterpret_cast<const char*>(buffer), bytes);
    SAGE_ASSERT(!m_Filestream.fail(), "[SYSTEM] Failed to write into the file '{}'", SAGEFile::GetFilepath().string());
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
    return SAGEFile::GetFilepath();
  }

  bool DiskFile::GetLine(std::string& line)
  {
    if (std::getline(m_Filestream, line))
      return true;
    return false;
  }

  DiskFile::operator bool() const
  {
    return m_Filestream.good() && m_Filestream.is_open() && !SAGEFile::GetFilepath().empty();
  }

  RAMFile::RAMFile()
    : SAGEFile(), m_Position(0) {}

  RAMFile::RAMFile(const std::filesystem::path& filepath)
    : SAGEFile(filepath), m_Position(0)
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

  RAMFile::RAMFile(const std::filesystem::path& filepath, const std::vector<char>& data)
    : SAGEFile(filepath), m_Position(0), m_Data(data.begin(), data.end()) {}

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

  int32_t RAMFile::GetSize()
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");
    return m_Data.size();
  }

  int32_t RAMFile::GetPosition()
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");
    return m_Position;
  }

  bool RAMFile::IsEndOfFile()
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");
    return m_Position == m_Data.size();
  }

  const std::filesystem::path& RAMFile::GetFilepath() const
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");
    return SAGEFile::GetFilepath();
  }

  bool RAMFile::GetLine(std::string& line)
  {
    SAGE_ASSERT(!m_Data.empty(), "[SYSTEM] The file is not good");

    if (m_Position >= m_Data.size())
      return false;

    int32_t start = m_Position;

    while (m_Position < m_Data.size() && m_Data[m_Position] != '\n')
      m_Position++;

    line.assign(m_Data.begin() + start, m_Data.begin() + m_Position);

    if (m_Position < m_Data.size() && m_Data[m_Position] == '\n')
      m_Position++;

    return true;
  }

  RAMFile::operator bool() const
  {
    return !m_Data.empty() && !SAGEFile::GetFilepath().empty();
  }

  ArchiveFile::ArchiveFile(const std::filesystem::path& filepath)
    : m_ArchiveFile(filepath, std::ios::in | std::ios::binary)
  {
    {
      std::string identifier;
      identifier.resize(4);
      m_ArchiveFile.Read(identifier.data(), 4);
      SAGE_ASSERT(identifier == "BIGF", "[SYSTEM] Error reading file identifier in file '{}'", filepath.string());
    }

    {
      int32_t archiveFileSize = 0;
      m_ArchiveFile.Read(&archiveFileSize, 4);
      SAGE_ASSERT(archiveFileSize == m_ArchiveFile.GetSize(), "[SYSTEM] The archive file size specified int it is not coherent with actual size of the file '{}'", filepath.string());
    }

    int32_t archiveFileCount = 0;
    m_ArchiveFile.Read(&archiveFileCount, 4);
    archiveFileCount = std::byteswap(archiveFileCount);

    m_ArchiveFile.Seek(16, SeekMode::Start);

    SAGE_INFO("Archive: {} | Size: {} | FileCount: {}", filepath.string(), m_ArchiveFile.GetSize(), archiveFileCount);

    for (int32_t i = 0; i < archiveFileCount; i++)
    {
      int32_t fileSize = 0;
      int32_t fileOffset = 0;
      m_ArchiveFile.Read(&fileOffset, 4);
      m_ArchiveFile.Read(&fileSize, 4);
      fileSize = std::byteswap(fileSize);
      fileOffset = std::byteswap(fileOffset);

      std::string archiveFilePath;
      int32_t pathIndex = -1;
      do
      {
        pathIndex++;
        archiveFilePath.resize(pathIndex + 1);
        m_ArchiveFile.Read(&archiveFilePath[pathIndex], 1);
      } while (archiveFilePath[pathIndex] != '\0');

      m_FileInfoList.insert(ArchivedFileInfo{ archiveFilePath , static_cast<uint32_t>(fileOffset), static_cast<uint32_t>(fileSize) });

      SAGE_INFO("\t\t\tFile: {} | Size: {} | Offset: {}", archiveFilePath, fileSize, fileOffset);
    }
  }

  RAMFile ArchiveFile::DumpFile(const std::filesystem::path& filepath)
  {
    for (auto it = m_FileInfoList.begin(); it != m_FileInfoList.end(); it++)
    {
      if (std::strcmp(it->Filepath.string().c_str(), filepath.string().c_str()) == 0)
      {
        std::vector<char> data;
        data.resize(it->Size + 1);
        m_ArchiveFile.Seek(it->Offset, SeekMode::Start);
        m_ArchiveFile.Read(data.data(), data.size());
        data[data.size() - 1] = '\0';
        return RAMFile(filepath, data);
      }
    }
    return RAMFile();
  }

  int32_t ArchiveFile::GetSize()
  {
    return m_ArchiveFile.GetSize();
  }

  int32_t ArchiveFile::GetFileCount() const
  {
    SAGE_ASSERT(m_ArchiveFile, "[SYSTEM] The file is not good")
    return m_FileInfoList.size();
  }
  const std::filesystem::path& ArchiveFile::GetFilepath() const
  {
    return m_ArchiveFile.GetFilepath();
  }

  ArchiveFile::operator bool() const
  {
    return m_ArchiveFile;
  }
}