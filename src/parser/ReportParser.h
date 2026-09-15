#pragma once

#include <optional>
#include <string>

#include "../models/Report.h"

class ReportParser {

    public:
        std::optional<Report>parseFilename(const std::string &filename) const;

    private:
        bool isValidDate(int day, int month, int year) const;

        bool isLeapYear(int year) const;

        std::string extractDdMmYyyyDate(const std::string &filename) const;

        std::string extractYyyyMmDdDate(const std::string &filename) const;

};

