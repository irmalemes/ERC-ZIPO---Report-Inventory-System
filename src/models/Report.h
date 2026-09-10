#pragma once

#include <string>
#include "ReportCategory.h"
#include "ReportType.h"

class Report
{
private:
    int id;
    std::string filename;
    ReportCategory category;
    ReportType reportType;
    std::string reportDate;
    std::string uploadDate;
    long long fileSize;
    std::string filePath;

public:
    Report(
        const std::string& filename,
        ReportCategory category,
        ReportType reportType,
        const std::string& reportDate,
        const std::string& uploadDate,
        long long fileSize,
        const std::string& filePath
    );


    int getId() const;
    void setId(int id);
    std::string getFilename() const;
    ReportCategory getCategory() const;
    ReportType getReportType() const;
    std::string getReportDate() const;
    std::string getUploadDate() const;
    long long getFileSize() const;
    std::string getFilePath() const;
};