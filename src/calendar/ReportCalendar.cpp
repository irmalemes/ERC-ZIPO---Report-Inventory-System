#include "ReportCalendar.h"
#include <stdexcept>

ReportCalendar::ReportCalendar() {

    expectedReportTypes = {
        ReportType::FZ_MIOPIO_SARAJEVO,
        ReportType::FZ_MIOPIO_MOSTAR,
        ReportType::ZAPISNIK_BPRV,
        ReportType::HALCOM_REPORT,
        ReportType::CBBH_INFRASTRUCTURE_REPORT
    };
}

std::vector<ReportType>ReportCalendar::getExpectedReportTypes() const {
    return expectedReportTypes;
}

std::string ReportCalendar::getReportTypeName(ReportType reportType) const {
    
    switch(reportType){

        case ReportType::FZ_MIOPIO_SARAJEVO:
            return "FZ MIOPIO SARAJEVO";

        case ReportType::FZ_MIOPIO_MOSTAR:
            return "FZ MIOPIO MOSTAR";

        case ReportType::ZAPISNIK_BPRV:
            return "ZAPISNIK BPRV";

        case ReportType::HALCOM_REPORT:
            return "HALCOM REPORT";

        case ReportType::CBBH_INFRASTRUCTURE_REPORT:
            return "CBBH INFRASTRUCTURE REPORT";
    }

    return "NEPOZNAT IZVJESTAJ";
}


ReportCategory ReportCalendar::getCategoryForType(ReportType reportType) const {

    switch(reportType) {

        case ReportType::FZ_MIOPIO_SARAJEVO:
        case ReportType::FZ_MIOPIO_MOSTAR:
            return ReportCategory::PIO_MIO;

        case ReportType::ZAPISNIK_BPRV:
            return ReportCategory::CBBH;

        case ReportType::HALCOM_REPORT:
        case ReportType::CBBH_INFRASTRUCTURE_REPORT:
            return ReportCategory::HALCOM;
    }

    return ReportCategory::PIO_MIO;
}

std::string ReportCalendar::getCategoryName(ReportCategory category) const {

    switch(category) {

        case ReportCategory::PIO_MIO:
            return "PIO/MIO";

        case ReportCategory::CBBH:
            return "CBBH";

        case ReportCategory::HALCOM:
            return "HALCOM";
    }

    return "NEPOZNATA KATEGORIJA";
}

std::string ReportCalendar::getStatusName(ReportPeriodStatus status) const {

    switch(status) {

        case ReportPeriodStatus::COMPLETE:
            return "COMPLETE";

        case ReportPeriodStatus::PARTIAL:
            return "PARTIAL";

        case ReportPeriodStatus::MISSING:
            return "MISSING";
    }

    return "UNKNOW";
}

ReportPeriod ReportCalendar::getReportPeriod(int day) const {
    if(day<=15) {
        return ReportPeriod::FIRST_HALF;
    } else {
        return ReportPeriod::SECOND_HALF;
    }
}

int ReportCalendar::getDayFromDate(const std::string &date) const {
    if(date.size() < 2) return 0;
    try {
       return std::stoi(date.substr(0, 2));
    } catch(const std::exception &error) {
        return 0;
    }   
}

int ReportCalendar::getMonthFromDate(const std::string &date) const {
    if(date.size() < 5) return 0;
    try {
        return std::stoi(date.substr(3, 2));
    } catch(const std::exception &error) {
        return 0;
    }
}

int ReportCalendar::getYearFromDate(const std::string &date) const {
    if(date.size() < 10) return 0;
    try {
        return std::stoi(date.substr(6, 4));
    } catch(const std::exception &error) {
        return 0;
    }
    
}

std::vector<Report>ReportCalendar::filterByPeriod(const std::vector<Report>&reports, ReportPeriod period, int month, int year) const {    

    std::vector<Report>filteredReports;

    for(const Report &report : reports) {
        int day = getDayFromDate(report.getReportDate());
        int reportMonth = getMonthFromDate(report.getReportDate());
        int reportYear = getYearFromDate(report.getReportDate());

        if(day < 1 || day > 31 || reportMonth < 1 || reportMonth > 12 || reportYear<=0) continue;

        if(getReportPeriod(day) == period && reportMonth == month && reportYear == year) {
            filteredReports.push_back(report);
        }
    }

    return filteredReports;
} 







ReportPeriodResult ReportCalendar::analyzeReports(const std::vector<Report>&reports) const {

    ReportPeriodResult result;

    result.expectedReports = expectedReportTypes.size();
    result.receivedReports = 0;
    result.missingReports = 0;
    result.completionPercentage = 0.0;
    result.status = ReportPeriodStatus::MISSING;

    for(ReportType expectedType : expectedReportTypes) {
        bool found = false;

        for(const Report &report : reports) {
            
            if(report.getReportType() == expectedType) {
                found = true;
                break;
            }
        }

        if(found) {
            result.receivedReports++;
        } else {
            result.missingReportTypes.push_back(expectedType);
        }
    }

    result.missingReports = result.expectedReports - result.receivedReports;

    if(result.expectedReports > 0) {
        result.completionPercentage = (static_cast<double>(result.receivedReports) / result.expectedReports) * 100.0;
    }

    if(result.receivedReports == 0) {
        result.status = ReportPeriodStatus::MISSING;
    } else if(result.receivedReports == result.expectedReports) {
        result.status = ReportPeriodStatus::COMPLETE;
    } else {
        result.status = ReportPeriodStatus::PARTIAL;
    }

    return result;
}

ReportPeriodResult ReportCalendar::analyzePeriod(const std::vector<Report>&reports, ReportPeriod period, int month, int year) const {

    std::vector<Report>filteredReports = filterByPeriod(reports, period, month, year);

    return analyzeReports(filteredReports);
}