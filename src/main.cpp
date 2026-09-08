#include <iostream>
#include "ReportInventory.h"

int main()
{
    std::cout << "=== Report Inventory System ===\n\n";

    // Kreiranje inventara
    ReportInventory inventory;

    // Testni PIO/MIO izvjestaj
    Report report1(
        "Izvjestaj FZ MIOPIO SARAJEVO 12.09.2026.pdf",
        ReportCategory::PIO_MIO,
        ReportType::FZ_MIOPIO_SARAJEVO,
        "12.09.2026",
        "12.09.2026",
        250000,
        "reports/PIO_MIO/Izvjestaj FZ MIOPIO SARAJEVO 12.09.2026.pdf"
    );

    // Testni CBBH izvjestaj
    Report report2(
        "Zapisnik pregleda sistema BPRV 12.09.2026.pdf",
        ReportCategory::CBBH,
        ReportType::ZAPISNIK_BPRV,
        "12.09.2026",
        "12.09.2026",
        180000,
        "reports/CBBH/Zapisnik pregleda sistema BPRV 12.09.2026.pdf"
    );

    // Testni HALCOM izvjestaj
    Report report3(
        "report 12.09.2026.pdf",
        ReportCategory::HALCOM,
        ReportType::HALCOM_REPORT,
        "12.09.2026",
        "12.09.2026",
        320000,
        "reports/HALCOM/report 12.09.2026.pdf"
    );

    // Dodavanje izvjestaja
    std::cout << "Dodavanje izvjestaja:\n";

    std::cout << (inventory.addReport(report1)
        ? "Report 1 uspjesno dodan.\n"
        : "Report 1 vec postoji.\n");

    std::cout << (inventory.addReport(report2)
        ? "Report 2 uspjesno dodan.\n"
        : "Report 2 vec postoji.\n");

    std::cout << (inventory.addReport(report3)
        ? "Report 3 uspjesno dodan.\n"
        : "Report 3 vec postoji.\n");

    std::cout << "\nBroj izvjestaja: "
              << inventory.getReportCount() << "\n";

    // Testiranje duplikata
    std::cout << "\nPokusaj dodavanja duplikata:\n";

    if (!inventory.addReport(report1))
    {
        std::cout << "Duplikat je prepoznat i nije dodan.\n";
    }

    // Pretraga
    std::cout << "\nPretraga izvjestaja:\n";

    const Report* foundReport =
        inventory.findReport("report 12.09.2026.pdf");

    if (foundReport != nullptr)
    {
        std::cout << "Pronadjen: "
                  << foundReport->getFilename() << "\n";
    }
    else
    {
        std::cout << "Izvjestaj nije pronadjen.\n";
    }

    // Filtriranje
    std::cout << "\nHALCOM izvjestaji:\n";

    std::vector<Report> halcomReports =
        inventory.filterByCategory(ReportCategory::HALCOM);

    for (const Report& report : halcomReports)
    {
        std::cout << "- " << report.getFilename() << "\n";
    }

    // Brisanje
    std::cout << "\nBrisanje CBBH izvjestaja:\n";

    if (inventory.removeReport(
            "Zapisnik pregleda sistema BPRV 12.09.2026.pdf"))
    {
        std::cout << "Izvjestaj uspjesno obrisan.\n";
    }
    else
    {
        std::cout << "Izvjestaj nije pronadjen.\n";
    }

    std::cout << "\nBroj izvjestaja nakon brisanja: "
              << inventory.getReportCount() << "\n";

    return 0;
}