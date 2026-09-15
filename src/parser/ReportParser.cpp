#include "ReportParser.h"

#include <regex>

std::optional<Report> ReportParser::parseFilename(const std::string &filename) const {

    std::string reportDate;

    //PIO/MIO Sarajevo
    if(filename.find("Izvještaj FZ MIOPIO SARAJEVO") != std::string::npos || 
       filename.find("Izvjestaj FZ MIOPIO SARAJEVO") != std::string::npos) {

        reportDate = extractDdMmYyyyDate(filename);

        if(reportDate.empty()){
            return std::nullopt;
        }

        return Report(
            filename,
            ReportCategory::PIO_MIO,
            ReportType::FZ_MIOPIO_SARAJEVO,
            reportDate,
            "",
            0,
            ""
        );
    }


    //PIO/MIO Mostar
    if(filename.find("Izvještaj FZ MIOPIO MOSTAR") != std::string::npos ||
       filename.find("Izvjestaj FZ MIOPIO MOSTAR") != std::string::npos) {

        reportDate = extractDdMmYyyyDate(filename);

        if(reportDate.empty()){
            return std::nullopt;
        }

        return Report(
            filename,
            ReportCategory::PIO_MIO,
            ReportType::FZ_MIOPIO_MOSTAR,
            reportDate,
            "",
            0,
            ""
        );
    }


    //CBBH BPRV
    if(filename.find("Zapisnik pregleda sistema BPRV") != std::string::npos) {

        reportDate = extractDdMmYyyyDate(filename);

        if(reportDate.empty()){
            return std::nullopt;
        }

        return Report(
            filename,
            ReportCategory::CBBH,
            ReportType::ZAPISNIK_BPRV,
            reportDate,
            "",
            0,
            ""
        );
    }


    //Halcom report
    if(filename.find("report ") == 0) {

        reportDate = extractDdMmYyyyDate(filename);

        if(reportDate.empty()){
            return std::nullopt;
        }

        return Report(
            filename,
            ReportCategory::HALCOM,
            ReportType::HALCOM_REPORT,
            reportDate,
            "",
            0,
            ""
        );
    }


    //CBBH Infrastructure report
    if(filename.find("CBBH Infrastructure Report") != std::string::npos) {

        reportDate = extractYyyyMmDdDate(filename);

        if(reportDate.empty()){
            return std::nullopt;
        }

        return Report(
            filename,
            ReportCategory::HALCOM,
            ReportType::CBBH_INFRASTRUCTURE_REPORT,
            reportDate,
            "",
            0,
            ""
        );
    }

    return std::nullopt;
}


std::string ReportParser::extractDdMmYyyyDate(const std::string &filename) const {

    std::regex datePattern(R"((\d{2})\.(\d{2})\.(\d{4}))");
    std::smatch match;

    if(!std::regex_search(filename, match, datePattern)){
        return "";
    }

    int day = std::stoi(match[1].str());
    int month = std::stoi(match[2].str());
    int year = std::stoi(match[3].str());

    if(!isValidDate(day, month, year)){
        return "";
    }

    return match[0].str();
}


std::string ReportParser::extractYyyyMmDdDate(const std::string &filename) const {

    std::regex datePattern(R"((\d{4})-(\d{2})-(\d{2}))");
    std::smatch match;

    if(!std::regex_search(filename, match, datePattern)){
        return "";
    }

    int year = std::stoi(match[1].str());
    int month = std::stoi(match[2].str());
    int day = std::stoi(match[3].str());

    if(!isValidDate(day, month, year)){
        return "";
    }

    return match[0].str();
}


bool ReportParser::isValidDate(
    int day,
    int month,
    int year
) const {

    if(year < 1900 || month < 1 || month > 12 || day < 1){
        return false;
    }

    int daysInMonth[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if(month == 2 && isLeapYear(year)){
        daysInMonth[1] = 29;
    }

    return day <= daysInMonth[month - 1];
}


bool ReportParser::isLeapYear(int year) const {

    return (year % 400 == 0) ||
           (year % 4 == 0 && year % 100 != 0);
}


