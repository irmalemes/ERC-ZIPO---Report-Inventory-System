#include "ReportInventory.h"

bool ReportInventory::addReport(const Report& report)
{
    for (const Report& existingReport : reports)
    {
        if (existingReport.getFilename() == report.getFilename() &&
            existingReport.getReportDate() == report.getReportDate())
        {
            return false;
        }
    }

    reports.push_back(report);
    return true;
}

bool ReportInventory::removeReport(const std::string& filename)
{
    for (auto it = reports.begin(); it != reports.end(); ++it)
    {
        if (it->getFilename() == filename)
        {
            reports.erase(it);
            return true;
        }
    }

    return false;
}

const Report* ReportInventory::findReport(const std::string& filename) const
{
    for (const Report& report : reports)
    {
        if (report.getFilename() == filename)
        {
            return &report;
        }
    }

    return nullptr;
}

std::vector<Report> ReportInventory::filterByCategory(
    ReportCategory category
) const
{
    std::vector<Report> filteredReports;

    for (const Report& report : reports)
    {
        if (report.getCategory() == category)
        {
            filteredReports.push_back(report);
        }
    }

    return filteredReports;
}

std::size_t ReportInventory::getReportCount() const
{
    return reports.size();
}

const std::vector<Report>& ReportInventory::getReports() const
{
    return reports;
}