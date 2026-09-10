#include <cassert>
#include <filesystem>
#include <iostream>

#include "../src/database/DatabaseManager.h"
#include "../src/models/Report.h"

int main() {

    const std::string testDatabasePath = "test_data/test_report_inventory.db";

    if(std::filesystem::exists(testDatabasePath)){
        std::filesystem::remove(testDatabasePath);
    }

    DatabaseManager database(testDatabasePath);

    assert(database.open());
    assert(database.createReportsTable());

    std::cout << "Database test setup uspjesan.\n";


    // INSERT i SELECT
    Report testReport(
        "Izvjestaj FZ MIOPIO SARAJEVO 12.09.2026.pdf",
        ReportCategory::PIO_MIO,
        ReportType::FZ_MIOPIO_SARAJEVO,
        "12.09.2026",
        "10.09.2026",
        2048,
        "reports/PIO_MIO/Izvjestaj FZ MIOPIO SARAJEVO 12.09.2026.pdf"
    );

    assert(database.insertReport(testReport));

    std::vector<Report> reports = database.getAllReports();

    assert(reports.size() == 1);
    assert(reports[0].getFilename() ==
           "Izvjestaj FZ MIOPIO SARAJEVO 12.09.2026.pdf");
    assert(reports[0].getReportDate() == "12.09.2026");
    assert(reports[0].getFileSize() == 2048);

    std::cout << "INSERT i SELECT test uspjesan.\n";


    // UPDATE
    Report updatedReport(
        "Izvjestaj FZ MIOPIO SARAJEVO 15.09.2026.pdf",
        reports[0].getCategory(),
        reports[0].getReportType(),
        "15.09.2026",
        reports[0].getUploadDate(),
        4096,
        "reports/PIO_MIO/Izvjestaj FZ MIOPIO SARAJEVO 15.09.2026.pdf"
    );

    updatedReport.setId(reports[0].getId());

    assert(database.updateReport(updatedReport));

    reports = database.getAllReports();

    assert(reports.size() == 1);
    assert(reports[0].getFilename() ==
           "Izvjestaj FZ MIOPIO SARAJEVO 15.09.2026.pdf");
    assert(reports[0].getReportDate() == "15.09.2026");
    assert(reports[0].getFileSize() == 4096);

    std::cout << "UPDATE test uspjesan.\n";


    // DELETE
    int idToDelete = reports[0].getId();

    assert(database.deleteReport(idToDelete));

    reports = database.getAllReports();

    assert(reports.empty());

    std::cout << "DELETE test uspjesan.\n";


    Report pioReport(
        "Izvjestaj FZ MIOPIO MOSTAR 22.09.2026.pdf",
        ReportCategory::PIO_MIO,
        ReportType::FZ_MIOPIO_MOSTAR,
        "22.09.2026",
        "20.09.2026",
        3000,
        "reports/PIO_MIO/Izvjestaj FZ MIOPIO MOSTAR 22.09.2026.pdf"
    );

    Report cbbhReport(
        "Zapisnik pregleda sistema BPRV 22.09.2026.pdf",
        ReportCategory::CBBH,
        ReportType::ZAPISNIK_BPRV,
        "22.09.2026",
        "20.09.2026",
        5000,
        "reports/CBBH/Zapisnik pregleda sistema BPRV 22.09.2026.pdf"
    );

    assert(database.insertReport(pioReport));
    assert(database.insertReport(cbbhReport));


    // SEARCH
    std::vector<Report> searchResults =
        database.searchByFilename("MOSTAR");

    assert(searchResults.size() == 1);
    assert(searchResults[0].getCategory() == ReportCategory::PIO_MIO);

    std::cout << "SEARCH test uspjesan.\n";


    // FILTER
    std::vector<Report> filteredReports =
        database.filterByCategory(ReportCategory::CBBH);

    assert(filteredReports.size() == 1);
    assert(filteredReports[0].getCategory() == ReportCategory::CBBH);

    std::cout << "FILTER test uspjesan.\n";


    // DUPLICATE
    bool duplicateInserted = database.insertReport(pioReport);

    assert(!duplicateInserted);

    std::vector<Report> afterDuplicate =
        database.searchByFilename("MOSTAR");

    assert(afterDuplicate.size() == 1);

    std::cout << "DUPLICATE test uspjesan.\n";

    database.close();


    // PERSISTENCE
    DatabaseManager reopenedDatabase(testDatabasePath);

    assert(reopenedDatabase.open());

    std::vector<Report> persistedReports =
        reopenedDatabase.getAllReports();

    assert(persistedReports.size() == 2);

    bool pioFound = false;
    bool cbbhFound = false;

    for(const Report &report : persistedReports){

        if(report.getFilename() ==
           "Izvjestaj FZ MIOPIO MOSTAR 22.09.2026.pdf"){

            pioFound = true;
        }

        if(report.getFilename() ==
           "Zapisnik pregleda sistema BPRV 22.09.2026.pdf"){

            cbbhFound = true;
        }
    }

    assert(pioFound);
    assert(cbbhFound);

    std::cout << "PERSISTENCE test uspjesan.\n";

    reopenedDatabase.close();

    if(std::filesystem::exists(testDatabasePath)){
        std::filesystem::remove(testDatabasePath);
    }

    std::cout << "Svi DatabaseManager testovi su uspjesno prosli.\n";

    return 0;
}