#include <iostream>
#include <filesystem>
#include <fstream>

#include "ReportInventory.h"
#include "FileManager.h"

int main()
{
    std::cout << "=== Report Inventory System ===\n\n";

    // =====================================================
    // MILESTONE 02 - REPORT INVENTORY
    // =====================================================

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
              << inventory.getReportCount()
              << "\n";

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
                  << foundReport->getFilename()
                  << "\n";
    }
    else
    {
        std::cout << "Izvjestaj nije pronadjen.\n";
    }

    // Filtriranje HALCOM izvjestaja
    std::cout << "\nHALCOM izvjestaji:\n";

    std::vector<Report> halcomReports =
        inventory.filterByCategory(ReportCategory::HALCOM);

    for (const Report& report : halcomReports)
    {
        std::cout << "- "
                  << report.getFilename()
                  << "\n";
    }

    // Brisanje CBBH izvjestaja
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
              << inventory.getReportCount()
              << "\n";


    // =====================================================
    // MILESTONE 03 - FILE MANAGER
    // =====================================================

    std::cout << "\n=== Milestone 03 - File Manager ===\n\n";

    FileManager fileManager;

    std::filesystem::path reportsDirectory = "reports";

    // Kreiranje osnovne strukture direktorija
    if (fileManager.createReportDirectories(reportsDirectory))
    {
        std::cout
            << "Folderi PIO_MIO, CBBH i HALCOM su kreirani.\n";
    }
    else
    {
        std::cout
            << "Greska pri kreiranju foldera.\n";
    }

    // Kreiranje testnog PDF fajla
    std::filesystem::path testFile = "test_report.pdf";

    {
        std::ofstream file(testFile);

        if (file)
        {
            file << "Testni dokument za FileManager.";
        }
        else
        {
            std::cerr
                << "Nije moguce kreirati testni fajl.\n";
        }
    }

    std::cout << "\nTestni fajl: "
              << testFile
              << "\n";

    // Provjera da li fajl postoji
    std::cout << "Postoji: "
              << (fileManager.fileExists(testFile)
                    ? "DA"
                    : "NE")
              << "\n";

    // Prikaz ekstenzije
    std::cout << "Ekstenzija: "
              << fileManager.getFileExtension(testFile)
              << "\n";

    // Provjera da li je format podrzan
    std::cout << "Podrzani format: "
              << (fileManager.isSupportedFile(testFile)
                    ? "DA"
                    : "NE")
              << "\n";

    // Prikaz velicine fajla
    std::uintmax_t fileSize =
        fileManager.getFileSize(testFile);

    std::cout << "Velicina: "
              << fileManager.formatFileSize(fileSize)
              << "\n";

    // Kopiranje fajla u PIO_MIO
    std::filesystem::path copiedFile =
        reportsDirectory /
        "PIO_MIO" /
        "test_report.pdf";

    if (fileManager.copyFile(testFile, copiedFile))
    {
        std::cout
            << "\nFajl uspjesno kopiran u PIO_MIO.\n";
    }
    else
    {
        std::cout
            << "\nKopiranje fajla nije uspjelo.\n";
    }

    // Premjestanje fajla iz PIO_MIO u CBBH
    std::filesystem::path movedFile =
        reportsDirectory /
        "CBBH" /
        "test_report.pdf";

    if (fileManager.moveFile(copiedFile, movedFile))
    {
        std::cout
            << "Fajl uspjesno premjesten u CBBH.\n";
    }
    else
    {
        std::cout
            << "Premjestanje fajla nije uspjelo.\n";
    }

    // Brisanje premjestenog fajla
    if (fileManager.deleteFile(movedFile))
    {
        std::cout
            << "Premjesteni fajl uspjesno obrisan.\n";
    }
    else
    {
        std::cout
            << "Brisanje premjestenog fajla nije uspjelo.\n";
    }

    // Brisanje originalnog testnog fajla
    if (fileManager.deleteFile(testFile))
    {
        std::cout
            << "Originalni testni fajl uspjesno obrisan.\n";
    }

    std::cout
        << "\nFileManager demonstracija zavrsena.\n";

    return 0;
}