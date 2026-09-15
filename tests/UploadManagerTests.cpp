#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../src/upload/UploadManager.h"

int main() {

    std::cout<<"Pokretanje UploadManager testova...\n";

    std::filesystem::path testDirectory = "test_data/upload_tests";
    std::filesystem::path testDatabase = testDirectory/"upload_tests.db";

    std::filesystem::remove_all(testDirectory);
    std::filesystem::create_directories(testDirectory);

    std::filesystem::path uploadedTestFile =
    "reports/HALCOM/report 20.09.2026.pdf";

    if(std::filesystem::exists(uploadedTestFile)){
        std::filesystem::remove(uploadedTestFile);
    }

    DatabaseManager database(testDatabase.string());

    assert(database.open());
    assert(database.createReportsTable());

    std::filesystem::path testFile =
    testDirectory / "report 20.09.2026.pdf";

    std::ofstream file(testFile);
    file << "Testni HALCOM izvjestaj";
    file.close();

    assert(std::filesystem::exists(testFile));
    assert(std::filesystem::file_size(testFile) > 0);

    UploadManager uploadManager(database);
    assert(uploadManager.isValidFile(testFile.string()));

    std::optional<Report> parsedReport =
    uploadManager.parseReport(testFile.string());

    assert(parsedReport.has_value());
    assert(parsedReport->getCategory() == ReportCategory::HALCOM);
    assert(parsedReport->getReportType() == ReportType::HALCOM_REPORT);
    assert(parsedReport->getReportDate() == "20.09.2026");
    std::cout<<"Validacija i parser: OK\n";

    Report report = parsedReport.value();
    report.setFilePath(testFile.string());
    assert(uploadManager.uploadReport(report));
    assert(database.reportExists(report.getFilename(), report.getReportDate()));
    assert(std::filesystem::exists(report.getFilePath()));
    assert(report.getFileSize()>0);
    std::cout<<"Upload i SQLite: OK\n";

    assert(uploadManager.isDuplicate(report.getFilename(), report.getReportDate()));
    assert(!uploadManager.uploadReport(report));
    std::cout<<"Zastita od duplikata: OK\n";

    std::filesystem::path invalidFile = testDirectory / "neispravan_izvjestaj.txt";
    std::ofstream invalid(invalidFile);
    invalid << "Ovo nije PDF ili DOCX.";
    invalid.close();
    assert(!uploadManager.isValidFile(invalidFile.string()));
    std::cout<<"Neispravan format: OK\n";

    if(std::filesystem::exists(uploadedTestFile)){
        std::filesystem::remove(uploadedTestFile);
    }

    database.close();
    std::filesystem::remove_all(testDirectory);
    std::cout<<"UploadManager testovi su uspjesno zavrseni.\n";
    






    return 0;
}

