#pragma once

#include <filesystem>
#include <fstream>

namespace sage
{
  enum class SeekMode
  {
    Start, Current, End
  };

  class DiskFile
  {
  public:
    DiskFile() = default;
    DiskFile(const std::filesystem::path& filepath, std::ios_base::openmode access);

    int32_t Read(void* buffer, int32_t bytes);
    void Write(const void* buffer, int32_t bytes);
    int32_t Seek(int32_t position, SeekMode mode);

    int32_t	GetSize();
    int32_t	GetPosition();
    bool IsEndOfFile();
    const std::filesystem::path& GetFilepath() const;

    bool GetLine(std::string& line);

  private:
    std::filesystem::path m_Filepath;
    std::fstream m_Filestream;
    std::ios_base::openmode m_Access;
  };

  class RAMFile
  {
  public:
    RAMFile() = default;
    RAMFile(const std::filesystem::path& filepath);

    int32_t Read(void* buffer, int32_t bytes);
    int32_t Seek(int32_t position, SeekMode mode);

    int32_t	GetSize() const;
    int32_t	GetPosition() const;
    bool IsEndOfFile() const;
    const std::filesystem::path& GetFilepath() const;

  private:
    std::filesystem::path m_Filepath;
    std::vector<char> m_Data;
    int32_t	m_Position;
  };
}