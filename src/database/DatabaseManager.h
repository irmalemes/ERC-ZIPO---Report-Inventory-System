#pragma once

#include <string>
#include <vector>

#include "../models/Report.h"

struct sqlite3;

class DatabaseManager {

    private: 
        sqlite3 *database;
        std::string databasePath;

    public:

        DatabaseManager(const std::string &databasePath);
        ~DatabaseManager();

        bool open();
        void close();

        bool createReportsTable();

        bool insertReport(const Report &report);

        std::vector<Report>getAllReports() const;

        bool updateReport(const Report &report);

        bool deleteReport(int id);

        std::vector<Report>searchByFilename(const std::string &filename) const;
        std::vector<Report>filterByCategory(ReportCategory category) const;
        bool reportExists(const std::string &filename, const std::string &reportDate) const;

};

