#include <cassert>
#include <iostream>

#include "../src/ReportInventory.h"

int main()
{
    ReportInventory inventory;

    Report report1(
        "report 12.09.2026.pdf",
        ReportCategory::HALCOM,
        ReportType::HALCOM_REPORT,
        "12.09.2026",
        "12.09.2026",
        320000,
        "reports/HALCOM/report 12.09.2026.pdf"
    );

    // Test 1: dodavanje
    assert(inventory.addReport(report1) == true);
    assert(inventory.getReportCount() == 1);

    // Test 2: duplikat
    assert(inventory.addReport(report1) == false);
    assert(inventory.getReportCount() == 1);

    // Test 3: pretraga
    const Report* foundReport =
        inventory.findReport("report 12.09.2026.pdf");

    assert(foundReport != nullptr);
    assert(foundReport->getFilename() == "report 12.09.2026.pdf");

    // Test 4: filtriranje
    std::vector<Report> halcomReports =
        inventory.filterByCategory(ReportCategory::HALCOM);

    assert(halcomReports.size() == 1);

    // Test 5: brisanje
    assert(inventory.removeReport("report 12.09.2026.pdf") == true);
    assert(inventory.getReportCount() == 0);

    // Test 6: brisanje nepostojeceg reporta
    assert(inventory.removeReport("nepostojeci.pdf") == false);

    std::cout << "Svi testovi su uspjesno prosli.\n";

    return 0;
}