#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <vector>
#include "../database/DatabaseManager.h"
#include "../upload/UploadManager.h"
#include "../calendar/ReportCalendar.h"
#include "../reminders/ReminderManager.h"
#include "../email/EmailManager.h"
#include "../email/SmtpClient.h"




class QPushButton;
class QLineEdit;
class QComboBox;
class QTableWidget;
class QLabel;


class MainWindow : public QMainWindow {

    public:
        MainWindow(QWidget *parent = nullptr);

    protected:
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dropEvent(QDropEvent *event) override; 

    private:
        QPushButton *addReportButton;
        QLineEdit *searchLineEdit;
        QComboBox *categoryComboBox;
        QTableWidget *reportTable;
        QLabel *reportCountLabel;
        QPushButton *themeButton;
        bool darkTheme = true;
        QComboBox *periodComboBox;
        QComboBox *monthComboBox;
        QSpinBox *yearSpinBox;
        QPushButton *periodOverviewButton; 
        QTableWidget *periodTable;
        QLabel *periodSummaryLabel;
        QLabel *reminderTitleLabel;
        QLabel *reminderDeadlineLabel;
        QLabel *reminderStatusLabel;
        QLabel *reminderMessageLabel;
        QLabel *cbbhReminderLabel;
        QPushButton *cbbhEmailButton;
        QLabel *dropHintLabel;


        DatabaseManager database;
        UploadManager uploadManager;
        ReportCalendar reportCalendar;
        ReminderManager reminderManager;
        EmailManager emailManager;
        SmtpClient smtpClient;
        void loadReports();
        void searchReports();
        void filterReports();
        void displayReports(const std::vector<Report>&reports);
        void applyTheme();
        void addReport();
        bool processReportFile(const QString &filePath);
        void showPeriodOverview();
        void updateReminderPanel();
        void openCbbhEmailForm();
};