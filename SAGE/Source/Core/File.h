#pragma once

#include <unordered_set>
#include <filesystem>
#include <fstream>

namespace sage
{
  enum class SeekMode
  {
    Start, Current, End
  };

  class SAGEFile
  {
  public:
    SAGEFile() = default;
    SAGEFile(const std::filesystem::path& filepath);

    virtual int32_t Read(void* buffer, int32_t bytes) = 0;
    virtual int32_t Seek(int32_t position, SeekMode mode) = 0;

    virtual int32_t	GetSize() = 0;
    virtual int32_t	GetPosition() = 0;
    virtual bool IsEndOfFile() = 0;
    virtual const std::filesystem::path& GetFilepath() const { return m_Filepath; }
    virtual bool GetLine(std::string& line) = 0;

  private:
    std::filesystem::path m_Filepath;
  };

  class DiskFile : public SAGEFile
  {
  public:
    DiskFile();
    DiskFile(const std::filesystem::path& filepath, std::ios_base::openmode access);

    int32_t Read(void* buffer, int32_t bytes) override;
    void Write(const void* buffer, int32_t bytes);
    int32_t Seek(int32_t position, SeekMode mode) override;

    int32_t	GetSize() override;
    int32_t	GetPosition() override;
    bool IsEndOfFile() override;
    const std::filesystem::path& GetFilepath() const override;
    bool GetLine(std::string& line) override;

    operator bool() const;

  private:
    std::fstream m_Filestream;
    std::ios_base::openmode m_Access;
  };

  class RAMFile : public SAGEFile
  {
  public:
    RAMFile();
    RAMFile(const std::filesystem::path& filepath);
    RAMFile(const std::filesystem::path& filepath, const std::vector<char>& data);

    int32_t Read(void* buffer, int32_t bytes) override;
    int32_t Seek(int32_t position, SeekMode mode) override;

    int32_t	GetSize() override;
    int32_t	GetPosition() override;
    bool IsEndOfFile() override;
    const std::filesystem::path& GetFilepath() const override;
    bool GetLine(std::string& line) override;

    operator bool() const;

  private:
    std::vector<char> m_Data;
    int32_t	m_Position;
  };

  struct ArchivedFileInfo
  {
    std::filesystem::path Filepath;
    uint32_t Offset;
    uint32_t Size;

    bool operator==(const ArchivedFileInfo& other) const { return Filepath == other.Filepath; }
    std::size_t operator()(const ArchivedFileInfo& file) const { return std::hash<std::string>{}(file.Filepath.string()); }
  };

  class ArchiveFile
  {
  public:
    ArchiveFile() = default;
    ArchiveFile(const std::filesystem::path& filepath);

    RAMFile DumpFile(const std::filesystem::path& filepath);

    int32_t	GetSize();
    int32_t GetFileCount() const;
    const std::filesystem::path& GetFilepath() const;

    operator bool() const;

  private:
    using ArchivedFileInfoList = std::unordered_set<ArchivedFileInfo, ArchivedFileInfo>;

  private:
    DiskFile m_ArchiveFile;
    ArchivedFileInfoList m_FileInfoList;
  };
}