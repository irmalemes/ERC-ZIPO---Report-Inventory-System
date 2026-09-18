#include <cassert>
#include <iostream>

#include "../src/calendar/ReportCalendar.h"

int main() {

    ReportCalendar calendar;
    std::vector<ReportType>expectedTypes = calendar.getExpectedReportTypes();
    assert(expectedTypes.size() == 5);
    std::cout<<"Broj ocekivanih izvjestaja: OK\n";

    std::vector<Report>emptyReports;
    ReportPeriodResult missingResult = calendar.analyzeReports(emptyReports);
    assert(missingResult.expectedReports == 5);
    assert(missingResult.receivedReports == 0);
    assert(missingResult.missingReports == 5);
    assert(missingResult.completionPercentage == 0.0);
    assert(missingResult.status == ReportPeriodStatus::MISSING);
    assert(missingResult.missingReportTypes.size() == 5);
    std::cout<<"MISSING status: OK\n";

    std::vector<Report>partialReports = {
        Report(
            "sarajevo.pdf",
            ReportCategory::PIO_MIO,
            ReportType::FZ_MIOPIO_SARAJEVO,
            "10.09.2026",
            "10.09.2026",
            100,
            "sarajevo.pdf"
        ),
        Report(
            "mostar.pdf",
            ReportCategory::PIO_MIO,
            ReportType::FZ_MIOPIO_MOSTAR,
            "10.09.2026",
            "10.09.2026",
            100,
            "mostar.pdf"
        ),
        Report(
            "bprv.pdf",
            ReportCategory::CBBH,
            ReportType::ZAPISNIK_BPRV,
            "10.09.2026",
            "10.09.2026",
            100,
            "bprv.pdf"
        ),
        Report(
            "halcom.pdf",
            ReportCategory::HALCOM,
            ReportType::HALCOM_REPORT,
            "10.09.2026",
            "10.09.2026",
            100,
            "halcom.pdf"
        )
    };

    ReportPeriodResult partialResult = calendar.analyzeReports(partialReports);
    assert(partialResult.expectedReports == 5);
    assert(partialResult.receivedReports == 4);
    assert(partialResult.missingReports == 1);
    assert(partialResult.completionPercentage == 80.0);
    assert(partialResult.status == ReportPeriodStatus::PARTIAL);
    assert(partialResult.missingReportTypes.size() == 1);
    assert(partialResult.missingReportTypes[0] == ReportType::CBBH_INFRASTRUCTURE_REPORT);
    std::cout<<"PARTIAL status 4/5: OK\n";


    std::vector<Report>completeReports = partialReports;
    completeReports.push_back(
        Report(
            "infrastructure.pdf",
            ReportCategory::HALCOM,
            ReportType::CBBH_INFRASTRUCTURE_REPORT,
            "10.09.2026",
            "10.09.2026",
            100,
            "infrastructure.pdf"
        )
    );

    ReportPeriodResult completeResult = calendar.analyzeReports(completeReports);
    assert(completeResult.expectedReports == 5);
    assert(completeResult.receivedReports == 5);
    assert(completeResult.missingReports == 0);
    assert(completeResult.completionPercentage == 100.0);
    assert(completeResult.status == ReportPeriodStatus::COMPLETE);
    assert(completeResult.missingReportTypes.empty());
    std::cout<<"COMPLETE status 5/5: OK\n";

    std::vector<Report>periodReports = {
        Report(
            "first.pdf",
            ReportCategory::PIO_MIO,
            ReportType::FZ_MIOPIO_SARAJEVO,
            "15.09.2026",
            "15.09.2026",
            100,
            "first.pdf"
        ),
        Report(
            "second.pdf",
            ReportCategory::PIO_MIO,
            ReportType::FZ_MIOPIO_MOSTAR,
            "16.09.2026",
            "16.09.2026",
            100,
            "second.pdf"
        )
    };

    std::vector<Report>firstHalf = calendar.filterByPeriod(periodReports, ReportPeriod::FIRST_HALF, 9, 2026);
    std::vector<Report>secondHalf = calendar.filterByPeriod(periodReports, ReportPeriod::SECOND_HALF, 9, 2026);
    assert(firstHalf.size() == 1);
    assert(secondHalf.size() == 1);
    assert(firstHalf[0].getReportDate() == "15.09.2026");
    assert(secondHalf[0].getReportDate() == "16.09.2026");
    std::cout<<"Filtriranje po periodu: OK\n";


    std::vector<Report>wrongPeriodReports = {
        Report(
            "wrong-month.pdf",
            ReportCategory::PIO_MIO,
            ReportType::FZ_MIOPIO_SARAJEVO,
            "10.08.2026",
            "10.08.2026",
            100,
            "wrong-month.pdf"

        ),
        Report(
            "wrong-year.pdf",
            ReportCategory::CBBH,
            ReportType::ZAPISNIK_BPRV,
            "10.09.2025",
            "10.09.2025",
            100,
            "wrong-year.pdf"
        )
    };

    std::vector<Report>wrongPeriodResult = calendar.filterByPeriod(wrongPeriodReports, ReportPeriod::FIRST_HALF, 9, 2026);
    assert(wrongPeriodResult.empty());
    std::cout<<"Filtriranje mjeseca i godine: OK\n";


    std::vector<Report>duplicateReports = {
        Report(
            "report1.pdf",
            ReportCategory::HALCOM,
            ReportType::HALCOM_REPORT,
            "10.09.2026",
            "10.09.2026",
            100,
            "report1.pdf"
        ),
        Report(
            "report2.pdf",
            ReportCategory::HALCOM,
            ReportType::HALCOM_REPORT,
            "12.09.2026",
            "12.09.2026",
            100,
            "report2.pdf"
        )
    };

    ReportPeriodResult duplicateResult = calendar.analyzeReports(duplicateReports);
    assert(duplicateResult.expectedReports == 5);
    assert(duplicateResult.receivedReports == 1);
    assert(duplicateResult.missingReports == 4);
    assert(duplicateResult.completionPercentage == 20.0);
    assert(duplicateResult.status == ReportPeriodStatus::PARTIAL);
    std::cout<<"Duplikati se ne broje vise puta: OK\n";
    
    std::vector<Report>invalidDateReports = {
        Report(
            "invalid.pdf",
            ReportCategory::CBBH,
            ReportType::ZAPISNIK_BPRV,
            "xx.09.2026",
            "10.09.2026",
            100,
            "invalid.pdf"
        )
    };

    std::vector<Report>invalidDateResult = calendar.filterByPeriod(invalidDateReports, ReportPeriod::FIRST_HALF, 9, 2026);
    assert(invalidDateResult.empty());
    std::cout<<"Neispravan datum: OK\n";

    return 0;
}