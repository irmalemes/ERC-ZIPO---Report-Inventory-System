#include <iomanip>
#include <sstream>

#include "ReminderManager.h"

ReminderManager::ReminderManager() {

}

bool ReminderManager::isReminderDay(int day) const {
    if(day == 12 || day == 22) {
        return true;
    }

    return false;
}

int ReminderManager::getNextDeadlineDay(int currentDay) const {
    if(currentDay <= 12) return 12;
    if(currentDay <= 22) return 22;
    return 12;
}

int ReminderManager::getDaysRemaining(int currentDay, int daysInCurrentMonth) const {
    if(currentDay <= 12) return 12 - currentDay;
    if(currentDay <= 22) return 22 - currentDay;
    return (daysInCurrentMonth - currentDay) + 12;
} 

bool ReminderManager::isDeadlinePassed(int currentDay) const {
    if(currentDay > 12 && currentDay <=15) return true;
    if(currentDay > 22) return true;
    return false;
}

ReminderResult ReminderManager::createReminder(int currentDay, int currentMonth, int currentYear, int daysInCurrentMonth, const ReportPeriodResult &periodResult) const {

    ReminderResult result;
    result.currentDay = currentDay;
    result.nextDeadlineDay = getNextDeadlineDay(currentDay);

    if(currentDay > 22) {
        if(currentMonth == 12) {
            result.nextDeadlineMonth = 1;
        } else {
            result.nextDeadlineMonth = currentMonth + 1;
        }
    } else {
        result.nextDeadlineMonth = currentMonth; 
    }

    if(currentDay > 22 && currentMonth == 12) result.nextDeadlineYear = currentYear + 1;
    else result.nextDeadlineYear = currentYear;

    std::ostringstream deadline;
    deadline << std::setfill('0') << std::setw(2) << result.nextDeadlineDay << "."
    << std::setw(2) << result.nextDeadlineMonth << "." << result.nextDeadlineYear;

    result.nextDeadline = deadline.str();

    result.daysRemaining = getDaysRemaining(currentDay, daysInCurrentMonth);
    result.isReminderDay = isReminderDay(currentDay);
    result.deadlinePassed = isDeadlinePassed(currentDay);
    result.reportStatus = periodResult.status;
    result.missingReports = periodResult.missingReports;
    result.cbbhAnnouncementNeeded = true;
    result.cbbhMessage = "NAPOMENA: Potrebno je napraviti najavu za Centralnu banku BiH";

    if(result.isReminderDay) {
        result.message = "Danas je rok za izradu izvjestaja.";
    } else if(result.deadlinePassed && result.missingReports > 0) {
        result.message = "UPOZORENJE: Rok je prosao, a izvjestaji nedostaju!";
    } else if(result.missingReports > 0) {
        result.message = "Potrebno je izraditi nedostajuce izvjestaje.";
    } else {
        result.message = "Svi izvjestaji za trenutni period su evidentirani.";
    }

    return result;
}





