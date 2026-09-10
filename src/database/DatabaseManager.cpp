#include "DatabaseManager.h"

#include <iostream>
#include <filesystem>

#include "sqlite3.h"

DatabaseManager::DatabaseManager(const std::string &databasePath)  : database(nullptr), databasePath(databasePath) {

}

DatabaseManager::~DatabaseManager() {

    close();

}


bool DatabaseManager::open() {

    std::filesystem::path path(databasePath);

    if(path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    int result = sqlite3_open(databasePath.c_str(), &database);

    if(result != SQLITE_OK) {

        std::cout<<"Greska pri otvaranju baze: "<<sqlite3_errmsg(database)<<"\n";
        sqlite3_close(database);
        database = nullptr;
        return false;

    }

    return true;
}


void DatabaseManager::close() {

    if(database != nullptr){

        sqlite3_close(database);
        database = nullptr;
    }
}


bool DatabaseManager::createReportsTable() {

    const char *sql = 
        "CREATE TABLE IF NOT EXISTS reports ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "filename TEXT NOT NULL,"
        "category INTEGER NOT NULL,"
        "report_type INTEGER NOT NULL,"
        "report_date TEXT NOT NULL,"
        "upload_date TEXT NOT NULL,"
        "file_size INTEGER NOT NULL,"
        "file_path TEXT NOT NULL"
        ");";

    char *errorMessage = nullptr;

    int result = sqlite3_exec(
        database,
        sql,
        nullptr,
        nullptr,
        &errorMessage
    );

    if(result != SQLITE_OK){

        std::cout<<"Greska pri kreiranju tabele: "<<errorMessage<<"\n";
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}


bool DatabaseManager::insertReport(const Report &report){

    if(reportExists(report.getFilename(), report.getReportDate())){

    std::cout << "Izvjestaj vec postoji u bazi.\n";
    return false;
}

    const char *sql = 
        "INSERT INTO reports ("
        "filename, category, report_type, report_date, "
        "upload_date, file_size, file_path"
        ") VALUES (?, ?, ?, ?, ?, ?, ?);";


    sqlite3_stmt *statement = nullptr;

    int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if(result != SQLITE_OK){

        std::cout<<"Greska pri pripremi INSERT upita: "<<sqlite3_errmsg(database)<<"\n";
        return false;
    }

    sqlite3_bind_text(statement, 1, report.getFilename().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(statement, 2, static_cast<int>(report.getCategory()));
    sqlite3_bind_int(statement, 3, static_cast<int>(report.getReportType()));
    sqlite3_bind_text(statement, 4, report.getReportDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 5, report.getUploadDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(statement, 6, report.getFileSize());
    sqlite3_bind_text(statement, 7, report.getFilePath().c_str(), -1, SQLITE_TRANSIENT);

    result = sqlite3_step(statement);

    if(result != SQLITE_DONE){
        std::cout<<"Greska pri INSERT-u: "<<sqlite3_errmsg(database)<<"\n";
        sqlite3_finalize(statement);
        return false;
    }

    sqlite3_finalize(statement);
    return true;
}

std::vector<Report>DatabaseManager::getAllReports() const {

    std::vector<Report>reports;

    const char *sql = "SELECT id, filename, category, report_type, report_date, "
        "upload_date, file_size, file_path FROM reports;";

    sqlite3_stmt *statement = nullptr;
    
    int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if(result != SQLITE_OK){
        std::cout<<"Greska pri SELECT upitu: "<<sqlite3_errmsg(database)<<"\n";
        return reports;
    }

    while(sqlite3_step(statement) == SQLITE_ROW){

        int id = sqlite3_column_int(statement, 0);
        std::string filename = reinterpret_cast<const char*>(sqlite3_column_text(statement,1));
        ReportCategory category = static_cast<ReportCategory>(sqlite3_column_int(statement,2));
        ReportType reportType = static_cast<ReportType>(sqlite3_column_int(statement,3));
        std::string reportDate = reinterpret_cast<const char*>(sqlite3_column_text(statement,4));
        std::string uploadDate = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
        long long fileSize = sqlite3_column_int64(statement,6);
        std::string filePath = reinterpret_cast<const char*>(sqlite3_column_text(statement,7));


        Report report(filename, category, reportType, reportDate, uploadDate, fileSize, filePath);

        report.setId(id);
        reports.push_back(report);
    }

    sqlite3_finalize(statement);
    return reports;
}


bool DatabaseManager::updateReport(const Report &report) {

    const char *sql = "UPDATE reports SET filename = ?, category = ?, report_type = ?, report_date = ?, "
        "upload_date = ?, file_size = ?, file_path = ? WHERE id = ?;";

    
    sqlite3_stmt *statement = nullptr;

    int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if(result != SQLITE_OK){
        std::cout<<"Greska pri pripremi UPDATE upita: "<<sqlite3_errmsg(database)<<"\n";
        return false;
    }

    sqlite3_bind_text(statement, 1, report.getFilename().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(statement, 2, static_cast<int>(report.getCategory()));
    sqlite3_bind_int(statement, 3, static_cast<int>(report.getReportType()));
    sqlite3_bind_text(statement, 4, report.getReportDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 5, report.getUploadDate().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(statement, 6, report.getFileSize());
    sqlite3_bind_text(statement, 7, report.getFilePath().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(statement, 8, report.getId());

    result = sqlite3_step(statement);

    if(result != SQLITE_DONE){
        std::cout<<"Greska pri UPDATE-u: "<<sqlite3_errmsg(database)<<"\n";
        sqlite3_finalize(statement);
        return false;
    }

    sqlite3_finalize(statement);
    return true;
}


bool DatabaseManager::deleteReport(int id){

    const char *sql = "DELETE FROM reports WHERE id = ?;";

    sqlite3_stmt *statement = nullptr;

    int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if(result != SQLITE_OK){
        std::cout<<"Greska pri pripremi DELETE upita: "<<sqlite3_errmsg(database)<<"\n";
        return false;
    }

    sqlite3_bind_int(statement, 1, id);
    result = sqlite3_step(statement);

    if(result != SQLITE_DONE){
        std::cout<<"Greska pri DELETE-u: "<<sqlite3_errmsg(database)<<"\n";
        sqlite3_finalize(statement);
        return false;
    }

    sqlite3_finalize(statement);
    return true;
}


std::vector<Report>DatabaseManager::searchByFilename(const std::string &filename) const {

    std::vector<Report>reports;

    const char *sql = "SELECT id, filename, category, report_type, report_date, "
        "upload_date, file_size, file_path FROM reports WHERE filename LIKE ?;";

    sqlite3_stmt *statement = nullptr;

    int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if(result != SQLITE_OK){
        std::cout<<"Greska pri SEARCH upitu: "<<sqlite3_errmsg(database)<<"\n";
        return reports;
    }

    std::string searchValue = "%" + filename + "%";

    sqlite3_bind_text(statement, 1, searchValue.c_str(), -1, SQLITE_TRANSIENT);

    while(sqlite3_step(statement) == SQLITE_ROW){

        int id = sqlite3_column_int(statement,0);
        std::string reportFilename = reinterpret_cast<const char*>(sqlite3_column_text(statement,1));
        ReportCategory category = static_cast<ReportCategory>(sqlite3_column_int(statement,2));
        ReportType reportType = static_cast<ReportType>(sqlite3_column_int(statement,3));
        std::string reportDate = reinterpret_cast<const char*>(sqlite3_column_text(statement,4));
        std::string uploadDate = reinterpret_cast<const char*>(sqlite3_column_text(statement,5));
        long long fileSize = sqlite3_column_int64(statement,6);
        std::string filePath = reinterpret_cast<const char*>(sqlite3_column_text(statement,7));


        Report report(reportFilename, category, reportType, reportDate, uploadDate, fileSize, filePath);
        report.setId(id);
        reports.push_back(report);
    }

    sqlite3_finalize(statement);
    return reports;
}


std::vector<Report> DatabaseManager::filterByCategory(ReportCategory category) const {

    std::vector<Report> reports;

    const char *sql =
        "SELECT id, filename, category, report_type, report_date, "
        "upload_date, file_size, file_path "
        "FROM reports WHERE category = ?;";

    sqlite3_stmt *statement = nullptr;

    int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if(result != SQLITE_OK){

        std::cout << "Greska pri FILTER upitu: "
                  << sqlite3_errmsg(database)
                  << "\n";

        return reports;
    }

    sqlite3_bind_int(statement, 1, static_cast<int>(category));

    while(sqlite3_step(statement) == SQLITE_ROW){

        int id = sqlite3_column_int(statement, 0);

        std::string filename =
            reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));

        ReportCategory reportCategory =
            static_cast<ReportCategory>(sqlite3_column_int(statement, 2));

        ReportType reportType =
            static_cast<ReportType>(sqlite3_column_int(statement, 3));

        std::string reportDate =
            reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));

        std::string uploadDate =
            reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));

        long long fileSize =
            sqlite3_column_int64(statement, 6);

        std::string filePath =
            reinterpret_cast<const char*>(sqlite3_column_text(statement, 7));

        Report report(
            filename,
            reportCategory,
            reportType,
            reportDate,
            uploadDate,
            fileSize,
            filePath
        );

        report.setId(id);
        reports.push_back(report);
    }

    sqlite3_finalize(statement);

    return reports;
}

bool DatabaseManager::reportExists(const std::string &filename, const std::string &reportDate) const {

    const char *sql =
        "SELECT COUNT(*) FROM reports "
        "WHERE filename = ? AND report_date = ?;";

    sqlite3_stmt *statement = nullptr;

    int result = sqlite3_prepare_v2(database, sql, -1, &statement, nullptr);

    if(result != SQLITE_OK){

        std::cout << "Greska pri provjeri duplikata: "
                  << sqlite3_errmsg(database)
                  << "\n";

        return false;
    }

    sqlite3_bind_text(statement, 1, filename.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(statement, 2, reportDate.c_str(), -1, SQLITE_TRANSIENT);

    bool exists = false;

    if(sqlite3_step(statement) == SQLITE_ROW){

        exists = sqlite3_column_int(statement, 0) > 0;
    }

    sqlite3_finalize(statement);

    return exists;
}