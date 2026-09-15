#include "UploadManager.h"
#include <filesystem>

#include "../FileManager.h"
#include "../parser/ReportParser.h"


UploadManager::UploadManager(DatabaseManager &database) : database(database) {
    

}


bool UploadManager::isValidFile(const std::string &filePath) const {

    FileManager fileManager;

    if(!fileManager.fileExists(filePath)){
        return false;
    }

    if(!fileManager.isSupportedFile(filePath)){
        return false;
    }

    return true;
}

bool UploadManager::isDuplicate(const std::string &filename, const std::string &reportDate) {

    return database.reportExists(filename, reportDate);
}


std::optional<Report>UploadManager::parseReport(const std::string &filePath){

    ReportParser parser;
    std::string filename = std::filesystem::path(filePath).filename().string();

    return parser.parseFilename(filename);

}


bool UploadManager::uploadReport(Report &report){

    FileManager fileManager;

    std::filesystem::path baseDirectory = "reports";
    std::filesystem::path categoryDirectory;

    switch(report.getCategory()){

        case ReportCategory::PIO_MIO:
            categoryDirectory = baseDirectory / "PIO_MIO";
            break;
        
        case ReportCategory::CBBH:
            categoryDirectory = baseDirectory / "CBBH";
            break;

        case ReportCategory::HALCOM:
            categoryDirectory = baseDirectory / "HALCOM";
            break;
    }

    if(!fileManager.createReportDirectories(baseDirectory)) {

        return false;

    }

    std::filesystem::path sourcePath = report.getFilePath();
    std::filesystem::path destinationPath = categoryDirectory / report.getFilename();
    
    if(fileManager.fileExists(destinationPath)) {
        return false;
    }

    if(isDuplicate(report.getFilename(), report.getReportDate())) {

        return false;
    }

    if(!fileManager.copyFile(sourcePath, destinationPath)){

        return false;
    }

    report.setFilePath(destinationPath.string());
    report.setFileSize(fileManager.getFileSize(destinationPath));

    if(!database.insertReport(report)) {

        fileManager.deleteFile(destinationPath);

        return false;
    }

    return true;

}