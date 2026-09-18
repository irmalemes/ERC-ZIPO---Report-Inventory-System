#pragma once

#include <string>
#include "../calendar/ReportCalendar.h"

struct ReminderResult {
    int currentDay;
    int nextDeadlineDay;
    int nextDeadlineMonth;
    int nextDeadlineYear;
    std::string nextDeadline;
    int daysRemaining;

    bool isReminderDay;
    bool deadlinePassed;
    bool cbbhAnnouncementNeeded;

    ReportPeriodStatus reportStatus;
    int missingReports;
    std::string message;
    std::string cbbhMessage;
};

class ReminderManager {

    public:
        ReminderManager();
        bool isReminderDay(int day) const;
        int getNextDeadlineDay(int currentDay) const;
        int getDaysRemaining(int currentDay, int daysInCurrentMonth) const;
        bool isDeadlinePassed(int currentDay) const;
        ReminderResult createReminder(int currentDay, int currentMonth, int currentYear, int daysInCurrentMonth, const ReportPeriodResult &periodResult) const;

};