#include <cassert>
#include <iostream>

#include "../src/reminders/ReminderManager.h"

int main() {

    ReminderManager reminderManager;

    assert(reminderManager.isReminderDay(12));
    assert(reminderManager.isReminderDay(22));
    assert(!reminderManager.isReminderDay(11));
    assert(!reminderManager.isReminderDay(21));
    std::cout<<"Prepoznavanje 12. i 22. dana: OK\n";


    assert(reminderManager.getNextDeadlineDay(11) == 12);
    assert(reminderManager.getDaysRemaining(11, 30) == 1);
    assert(reminderManager.getNextDeadlineDay(12) == 12);
    assert(reminderManager.getDaysRemaining(12, 30) == 0);
    assert(reminderManager.getNextDeadlineDay(18) == 22);
    assert(reminderManager.getDaysRemaining(18, 30) == 4);
    assert(reminderManager.getNextDeadlineDay(22) == 22);
    assert(reminderManager.getDaysRemaining(22, 30) == 0);
    assert(reminderManager.getNextDeadlineDay(23) == 12);
    assert(reminderManager.getDaysRemaining(23, 30) == 19);
    std::cout<<"Sljedeci rok i preostali dani: OK\n";

    assert(!reminderManager.isDeadlinePassed(12));
    assert(reminderManager.isDeadlinePassed(13));
    assert(reminderManager.isDeadlinePassed(15));
    assert(!reminderManager.isDeadlinePassed(16));
    assert(!reminderManager.isDeadlinePassed(22));
    assert(reminderManager.isDeadlinePassed(23));
    std::cout<<"Provjera isteka roka: OK\n";

    ReportPeriodResult partialResult;
    partialResult.expectedReports = 5;
    partialResult.receivedReports = 3;
    partialResult.missingReports = 2;
    partialResult.completionPercentage = 60.0;
    partialResult.status = ReportPeriodStatus::PARTIAL;

    ReminderResult reminder = reminderManager.createReminder(23, 9, 2026, 30, partialResult);

    assert(reminder.deadlinePassed);
    assert(reminder.missingReports == 2);
    assert(reminder.reportStatus == ReportPeriodStatus::PARTIAL);
    assert(reminder.nextDeadline == "12.10.2026");
    assert(reminder.daysRemaining == 19);
    assert(reminder.cbbhAnnouncementNeeded);
    assert(!reminder.cbbhMessage.empty());
    std::cout<<"PARTIAL reminder i CBBH napomena: OK\n";

    assert(reminder.message == "UPOZORENJE: Rok je prosao, a izvjestaji nedostaju!");
    std::cout << "Upozorenje nakon isteka roka: OK\n";

    ReportPeriodResult completeResult;
    completeResult.expectedReports = 5;
    completeResult.receivedReports = 5;
    completeResult.missingReports = 0;
    completeResult.completionPercentage = 100.0;
    completeResult.status = ReportPeriodStatus::COMPLETE;

    ReminderResult completeReminder = reminderManager.createReminder(18, 9, 2026, 30, completeResult);

    assert(completeReminder.reportStatus == ReportPeriodStatus::COMPLETE);
    assert(completeReminder.missingReports == 0);
    assert(!completeReminder.deadlinePassed);
    assert(completeReminder.message == "Svi izvjestaji za trenutni period su evidentirani.");
    std::cout << "COMPLETE reminder: OK\n";

    ReminderResult reminder12 = reminderManager.createReminder(12, 9, 2026, 30, partialResult);

    assert(reminder12.isReminderDay);
    assert(reminder12.nextDeadline == "12.09.2026");
    assert(reminder12.daysRemaining == 0);
    assert(!reminder12.deadlinePassed);
    assert(reminder12.message == "Danas je rok za izradu izvjestaja.");

    ReminderResult reminder22 = reminderManager.createReminder(22, 9, 2026, 30, partialResult);

    assert(reminder22.isReminderDay);
    assert(reminder22.nextDeadline == "22.09.2026");
    assert(reminder22.daysRemaining == 0);
    assert(!reminder22.deadlinePassed);
    assert(reminder22.message == "Danas je rok za izradu izvjestaja.");
    std::cout << "Reminder za 12. i 22. dan: OK\n";

    ReminderResult decemberReminder = reminderManager.createReminder(23, 12, 2026, 31, partialResult);

    assert(decemberReminder.nextDeadlineDay == 12);
    assert(decemberReminder.nextDeadlineMonth == 1);
    assert(decemberReminder.nextDeadlineYear == 2027);
    assert(decemberReminder.nextDeadline == "12.01.2027");
    assert(decemberReminder.daysRemaining == 20);
    std::cout << "Prelazak decembar - januar: OK\n";





    return 0;
}