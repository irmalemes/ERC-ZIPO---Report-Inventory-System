#include "Report.h"

Report::Report(
    const std::string& filename,
    ReportCategory category,
    ReportType reportType,
    const std::string& reportDate,
    const std::string& uploadDate,
    long long fileSize,
    const std::string& filePath
)
    : filename(filename),
      category(category),
      reportType(reportType),
      reportDate(reportDate),
      uploadDate(uploadDate),
      fileSize(fileSize),
      filePath(filePath)
{
}

std::string Report::getFilename() const
{
    return filename;
}

ReportCategory Report::getCategory() const
{
    return category;
}

ReportType Report::getReportType() const
{
    return reportType;
}

std::string Report::getReportDate() const
{
    return reportDate;
}

std::string Report::getUploadDate() const
{
    return uploadDate;
}

long long Report::getFileSize() const
{
    return fileSize;
}

std::string Report::getFilePath() const
{
    return filePath;
}