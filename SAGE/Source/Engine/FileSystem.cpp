#include <pch.h>

#include "Engine/FileSystem.h"

namespace sage
{
  void LocalFileSystem::Init(const std::filesystem::path& directory)
  {
    std::filesystem::current_path(directory);
  }

  DiskFile LocalFileSystem::OpenFile(const std::filesystem::path& filepath, std::ios_base::openmode access)
  {
    if (std::filesystem::exists(filepath))
      return DiskFile(filepath, access);
    return DiskFile();
  }

  RAMFile LocalFileSystem::OpenFile(const std::filesystem::path& filepath)
  {
    if (std::filesystem::exists(filepath))
      return RAMFile(filepath);
    return RAMFile();
  }

  FilenameList LocalFileSystem::GetFilenameListInDirectory(const std::filesystem::path& directory, const std::string& filter, bool search_subdirectories)
  {
    FilenameList list;

    try
    {
      for (const auto& entry : std::filesystem::directory_iterator(directory))
        if (entry.is_regular_file() && entry.path().filename().string().find(filter) != std::string::npos)
          list.insert(entry.path().string());

      if (search_subdirectories)
        for (const auto& entry : std::filesystem::directory_iterator(directory))
          if (entry.is_directory())
            LocalFileSystem::GetFilenameListInDirectory(entry.path(), filter, search_subdirectories);

      return list;
    }
    catch (const std::exception& e)
    {
      SAGE_ASSERT(false, "[SYSTEM] Error accessing directory: {}", e.what());
      list.clear();
      return list;
    }
  }

  void ArchiveFileSystem::Init()
  {
    FilenameList list = LocalFileSystem::GetFilenameListInDirectory(".", ".big", true);

    for (auto it = list.begin(); it != list.end(); it++)
      ArchiveFileSystem::Get().s_ArchiveList.push_back(ArchiveFile(it->c_str()));
  }

  RAMFile ArchiveFileSystem::OpenFile(const std::filesystem::path& filepath)
  {
    for (ArchiveFile& archive : ArchiveFileSystem::Get().s_ArchiveList)
    {
      RAMFile file = archive.DumpFile(filepath);
      if (file)
        return file;
    }
    return RAMFile();
  }

  void FileSystem::Init(const std::filesystem::path& directory)
  {
    LocalFileSystem::Init(directory);
    ArchiveFileSystem::Init();
  }

  DiskFile FileSystem::OpenDiskFile(const std::filesystem::path& filepath, std::ios_base::openmode access)
  {
    DiskFile file = LocalFileSystem::OpenFile(filepath, access);
    SAGE_ASSERT(file, "[SYSTEM] Invalid filepath specified: '{}'", filepath.string());
    return file;
  }

  RAMFile FileSystem::OpenRAMFile(const std::filesystem::path& filepath)
  {
    RAMFile file = LocalFileSystem::OpenFile(filepath);
    if (file)
      return file;
    else
    {
      file = ArchiveFileSystem::OpenFile(filepath);
      SAGE_ASSERT(file, "[SYSTEM] Invalid filepath specified: '{}'", filepath.string());
      return file;
    }
  }
}