#pragma once

#include <QMainWindow>
#include <QComboBox>
#include <QSpinBox>
#include <vector>
#include "../database/DatabaseManager.h"
#include "../upload/UploadManager.h"
#include "../calendar/ReportCalendar.h"




class QPushButton;
class QLineEdit;
class QComboBox;
class QTableWidget;
class QLabel;


class MainWindow : public QMainWindow {

    public:
        MainWindow(QWidget *parent = nullptr);

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


        DatabaseManager database;
        UploadManager uploadManager;
        ReportCalendar reportCalendar;
        void loadReports();
        void searchReports();
        void filterReports();
        void displayReports(const std::vector<Report>&reports);
        void applyTheme();
        void addReport();
        void showPeriodOverview();
};