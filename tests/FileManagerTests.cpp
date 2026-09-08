#include <cassert>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "../src/FileManager.h"

int main(){

    FileManager fileManager;
 
    const std::filesystem::path testDirectory = "test_reports";

    assert(fileManager.createReportDirectories(testDirectory));

    assert(std::filesystem::exists(testDirectory / "PIO_MIO"));
    assert(std::filesystem::exists(testDirectory / "CBBH"));
    assert(std::filesystem::exists(testDirectory / "HALCOM"));


    const std::filesystem::path pdfFile = testDirectory / "test_report.pdf";

    {
        std::ofstream file(pdfFile);
        file << "Testni PDF file za Milestone 03";

    }

    assert(fileManager.fileExists(pdfFile));

    assert(fileManager.getFileExtension(pdfFile) == ".pdf");

    assert(fileManager.isSupportedFile(pdfFile));
    assert(fileManager.isSupportedFile("test.docx"));
    assert(fileManager.isSupportedFile("test.PDF"));
    assert(fileManager.isSupportedFile("test.DOCX"));
    assert(!fileManager.isSupportedFile("test.txt"));

    const std::uintmax_t fileSize = fileManager.getFileSize(pdfFile);

    std::cout << "Velicina testnog fajla: "
              << fileManager.formatFileSize(fileSize) << "\n";
              
    

    assert(fileManager.formatFileSize(1024) == "1.00 KB");
    assert(fileManager.formatFileSize(1024*1024) == "1.00 MB");
    assert(fileManager.formatFileSize(1024ULL * 1024ULL * 1024ULL) == "1.00 GB");
    
    const std::filesystem::path copiedFile = testDirectory / "PIO_MIO" / "copied_reprot";
    
    assert(fileManager.copyFile(pdfFile, copiedFile));
    assert(fileManager.fileExists(copiedFile));

    const std::filesystem::path movedFile = testDirectory / "CBBH" / "moved_report.pdf";

    assert(fileManager.moveFile(copiedFile, movedFile));

    assert(!fileManager.fileExists(copiedFile));
    assert(fileManager.fileExists(movedFile));

    assert(fileManager.deleteFile(movedFile));
    assert(!fileManager.fileExists(movedFile));

    std::filesystem::remove_all(testDirectory);

    std::cout << "Svi testovi su uspjesno prosli.\n";

    return 0;
}

