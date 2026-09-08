#pragma once

#include <vector>
#include <string>
#include "models/Report.h"

class ReportInventory
{
private:
    std::vector<Report> reports;

public:
    bool addReport(const Report& report);

    bool removeReport(const std::string& filename);

    const Report* findReport(const std::string& filename) const;

    std::vector<Report> filterByCategory(ReportCategory category) const;

    std::size_t getReportCount() const;

    const std::vector<Report>& getReports() const;
};