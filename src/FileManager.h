#pragma once

#include <filesystem>
#include <string>

class FileManager
{
public:
    bool fileExists(const std::filesystem::path& filePath) const;

    std::string getFileExtension(
        const std::filesystem::path& filePath
    ) const;

    std::uintmax_t getFileSize(
        const std::filesystem::path& filePath
    ) const;

    std::string formatFileSize(
        std::uintmax_t bytes
    ) const;

    bool isSupportedFile(
        const std::filesystem::path& filePath
    ) const;

    bool copyFile(
        const std::filesystem::path& source,
        const std::filesystem::path& destination
    ) const;

    bool moveFile(
        const std::filesystem::path& source,
        const std::filesystem::path& destination
    ) const;

    bool deleteFile(
        const std::filesystem::path& filePath
    ) const;

    bool createReportDirectories(
        const std::filesystem::path& baseDirectory
    ) const;
};