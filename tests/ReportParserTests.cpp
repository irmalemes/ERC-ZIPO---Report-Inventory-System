#include <cassert>
#include <iostream>

#include "../src/parser/ReportParser.h"

int main() {

    ReportParser parser;


    //PIO/MIO Sarajevo
    auto sarajevoReport = parser.parseFilename(
        "Izvjestaj FZ MIOPIO SARAJEVO 12.09.2026.pdf"
    );

    assert(sarajevoReport.has_value());
    assert(sarajevoReport->getCategory() == ReportCategory::PIO_MIO);
    assert(sarajevoReport->getReportType() == ReportType::FZ_MIOPIO_SARAJEVO);
    assert(sarajevoReport->getReportDate() == "12.09.2026");


    //PIO/MIO Mostar
    auto mostarReport = parser.parseFilename(
        "Izvjestaj FZ MIOPIO MOSTAR 22.09.2026.docx"
    );

    assert(mostarReport.has_value());
    assert(mostarReport->getCategory() == ReportCategory::PIO_MIO);
    assert(mostarReport->getReportType() == ReportType::FZ_MIOPIO_MOSTAR);
    assert(mostarReport->getReportDate() == "22.09.2026");


    //CBBH BPRV
    auto cbbhReport = parser.parseFilename(
        "Zapisnik pregleda sistema BPRV 12.09.2026.pdf"
    );

    assert(cbbhReport.has_value());
    assert(cbbhReport->getCategory() == ReportCategory::CBBH);
    assert(cbbhReport->getReportType() == ReportType::ZAPISNIK_BPRV);
    assert(cbbhReport->getReportDate() == "12.09.2026");


    //Halcom report
    auto halcomReport = parser.parseFilename(
        "report 22.09.2026.pdf"
    );

    assert(halcomReport.has_value());
    assert(halcomReport->getCategory() == ReportCategory::HALCOM);
    assert(halcomReport->getReportType() == ReportType::HALCOM_REPORT);
    assert(halcomReport->getReportDate() == "22.09.2026");


    //CBBH Infrastructure Report
    auto infrastructureReport = parser.parseFilename(
        "CBBH Infrastructure Report 2026-09-12.pdf"
    );

    assert(infrastructureReport.has_value());
    assert(infrastructureReport->getCategory() == ReportCategory::HALCOM);
    assert(
        infrastructureReport->getReportType() ==
        ReportType::CBBH_INFRASTRUCTURE_REPORT
    );
    assert(infrastructureReport->getReportDate() == "2026-09-12");


    //Nepoznat naziv
    auto unknownReport = parser.parseFilename(
        "nepoznati_izvjestaj.pdf"
    );

    assert(!unknownReport.has_value());


    //Neispravan dan
    auto invalidDay = parser.parseFilename(
        "report 32.09.2026.pdf"
    );

    assert(!invalidDay.has_value());


    //Neispravan mjesec
    auto invalidMonth = parser.parseFilename(
        "report 12.13.2026.pdf"
    );

    assert(!invalidMonth.has_value());


    //Neispravan februar
    auto invalidFebruary = parser.parseFilename(
        "report 29.02.2025.pdf"
    );

    assert(!invalidFebruary.has_value());


    //Prestupna godina
    auto leapYearReport = parser.parseFilename(
        "report 29.02.2024.pdf"
    );

    assert(leapYearReport.has_value());
    assert(leapYearReport->getReportDate() == "29.02.2024");


    //Neispravan yyyy-mm-dd datum
    auto invalidInfrastructureDate = parser.parseFilename(
        "CBBH Infrastructure Report 2026-02-30.pdf"
    );

    assert(!invalidInfrastructureDate.has_value());


    std::cout << "Svi ReportParser testovi su uspjesno prosli.\n";

    return 0;
}