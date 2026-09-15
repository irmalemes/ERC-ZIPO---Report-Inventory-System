#pragma once

#include <string>
#include <optional>


#include "../models/Report.h"
#include "../database/DatabaseManager.h"

class UploadManager{

    public:
        UploadManager(DatabaseManager &database);

        bool isValidFile(const std::string &filePath) const;
        bool isDuplicate(const std::string &filePath, const std::string &reportDate);

        std::optional<Report>parseReport(const std::string &filePath);

        bool uploadReport(Report &report);


    private:
        DatabaseManager &database;
};