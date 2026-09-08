#include "FileManager.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cctype>

bool FileManager::fileExists(
    const std::filesystem::path& filePath
) const
{
    try
    {
        return std::filesystem::exists(filePath) &&
               std::filesystem::is_regular_file(filePath);
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cerr << "Greska pri provjeri fajla: "
                  << error.what() << "\n";
        return false;
    }
}

std::string FileManager::getFileExtension(
    const std::filesystem::path& filePath
) const
{
    return filePath.extension().string();
}

std::uintmax_t FileManager::getFileSize(
    const std::filesystem::path& filePath
) const
{
    try
    {
        if (!fileExists(filePath))
        {
            return 0;
        }

        return std::filesystem::file_size(filePath);
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cerr << "Greska pri citanju velicine fajla: "
                  << error.what() << "\n";
        return 0;
    }
}

std::string FileManager::formatFileSize(
    std::uintmax_t bytes
) const
{
    std::ostringstream output;
    output << std::fixed << std::setprecision(2);

    constexpr double KB = 1024.0;
    constexpr double MB = 1024.0 * 1024.0;
    constexpr double GB = 1024.0 * 1024.0 * 1024.0;

    if (bytes < 1024)
    {
        output << bytes << " B";
    }
    else if (bytes < 1024ULL * 1024ULL)
    {
        output << bytes / KB << " KB";
    }
    else if (bytes < 1024ULL * 1024ULL * 1024ULL)
    {
        output << bytes / MB << " MB";
    }
    else
    {
        output << bytes / GB << " GB";
    }

    return output.str();
}

bool FileManager::isSupportedFile(
    const std::filesystem::path& filePath
) const
{
    std::string extension = getFileExtension(filePath);

    std::transform(
        extension.begin(),
        extension.end(),
        extension.begin(),
        [](unsigned char character)
        {
            return static_cast<char>(std::tolower(character));
        }
    );

    return extension == ".pdf" || extension == ".docx";
}

bool FileManager::copyFile(
    const std::filesystem::path& source,
    const std::filesystem::path& destination
) const
{
    try
    {
        if (!fileExists(source))
        {
            std::cerr << "Izvorni fajl ne postoji.\n";
            return false;
        }

        std::filesystem::copy_file(
            source,
            destination,
            std::filesystem::copy_options::overwrite_existing
        );

        return true;
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cerr << "Greska pri kopiranju fajla: "
                  << error.what() << "\n";
        return false;
    }
}

bool FileManager::moveFile(
    const std::filesystem::path& source,
    const std::filesystem::path& destination
) const
{
    try
    {
        if (!fileExists(source))
        {
            std::cerr << "Izvorni fajl ne postoji.\n";
            return false;
        }

        std::filesystem::rename(source, destination);

        return true;
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cerr << "Greska pri premjestanju fajla: "
                  << error.what() << "\n";
        return false;
    }
}

bool FileManager::deleteFile(
    const std::filesystem::path& filePath
) const
{
    try
    {
        if (!fileExists(filePath))
        {
            std::cerr << "Fajl ne postoji.\n";
            return false;
        }

        return std::filesystem::remove(filePath);
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cerr << "Greska pri brisanju fajla: "
                  << error.what() << "\n";
        return false;
    }
}

bool FileManager::createReportDirectories(
    const std::filesystem::path& baseDirectory
) const
{
    try
    {
        std::filesystem::create_directories(
            baseDirectory / "PIO_MIO"
        );

        std::filesystem::create_directories(
            baseDirectory / "CBBH"
        );

        std::filesystem::create_directories(
            baseDirectory / "HALCOM"
        );

        return true;
    }
    catch (const std::filesystem::filesystem_error& error)
    {
        std::cerr << "Greska pri kreiranju direktorija: "
                  << error.what() << "\n";
        return false;
    }
}