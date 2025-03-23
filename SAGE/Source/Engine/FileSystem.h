#pragma once

#include <variant>

#include "Core/File.h"

namespace sage
{
  class LocalFileSystem
  {
  public:
    static void Init(const std::filesystem::path& directory);
    static DiskFile OpenFile(const std::filesystem::path& filepath, std::ios_base::openmode access);
    static RAMFile OpenFile(const std::filesystem::path& filepath);
    static FilenameList GetFilenameListInDirectory(const std::filesystem::path& directory, const std::string& filter, bool search_subdirectories);

  private:
    static LocalFileSystem& Get() { static LocalFileSystem instance; return instance; }

    LocalFileSystem() = default;
    LocalFileSystem(const LocalFileSystem&) = delete;
    LocalFileSystem& operator=(const LocalFileSystem&) = delete;
    LocalFileSystem(LocalFileSystem&&) = delete;
    LocalFileSystem& operator=(LocalFileSystem&&) = delete;
  };

  class ArchiveFileSystem
  {
  public:
    static void Init();
    static RAMFile OpenFile(const std::filesystem::path& filepath);

  private:
    static ArchiveFileSystem& Get() { static ArchiveFileSystem instance; return instance; }

    ArchiveFileSystem() = default;
    ArchiveFileSystem(const ArchiveFileSystem&) = delete;
    ArchiveFileSystem& operator=(const ArchiveFileSystem&) = delete;
    ArchiveFileSystem(ArchiveFileSystem&&) = delete;
    ArchiveFileSystem& operator=(ArchiveFileSystem&&) = delete;

  private:
    ArchiveList s_ArchiveList;
  };

  using FileType = std::variant<DiskFile, RAMFile>;
  class FileSystem
  {
  public:
    static void Init(const std::filesystem::path& directory);
    static DiskFile OpenDiskFile(const std::filesystem::path& filepath, std::ios_base::openmode access);
    static RAMFile OpenRAMFile(const std::filesystem::path& filepath);

  private:
    static FileSystem& Get() { static FileSystem instance; return instance; }

    FileSystem() = default;
    FileSystem(const FileSystem&) = delete;
    FileSystem& operator=(const FileSystem&) = delete;
    FileSystem(FileSystem&&) = delete;
    FileSystem& operator=(FileSystem&&) = delete;
  };
}