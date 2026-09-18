#pragma once

#include <vector>
#include <string>
#include "../models/ReportType.h"
#include "../models/Report.h"


enum class ReportPeriod {
    FIRST_HALF,
    SECOND_HALF
};

enum class ReportPeriodStatus {
    COMPLETE,
    PARTIAL,
    MISSING
};

struct ReportPeriodResult {
    int expectedReports;
    int receivedReports;
    int missingReports;
    double completionPercentage;
    ReportPeriodStatus status;
    std::vector<ReportType>missingReportTypes;
};

class ReportCalendar {

    public:
        ReportCalendar();
        std::vector<ReportType>getExpectedReportTypes() const;
        ReportPeriodResult analyzeReports(const std::vector<Report>&reports) const;
        ReportPeriodResult analyzePeriod(const std::vector<Report>&reports, ReportPeriod period, int month, int year) const;
        ReportPeriod getReportPeriod(int day) const;
        std::vector<Report>filterByPeriod(const std::vector<Report>&reports, ReportPeriod period, int month, int year) const;
        std::string getReportTypeName(ReportType reportType) const;
        ReportCategory getCategoryForType(ReportType reportType) const;
        std::string getCategoryName(ReportCategory category) const;
        std::string getStatusName(ReportPeriodStatus status) const;




    private:
        std::vector<ReportType>expectedReportTypes;
        int getDayFromDate(const std::string &date) const;
        int getMonthFromDate(const std::string &date) const;
        int getYearFromDate(const std::string &date) const;
};





