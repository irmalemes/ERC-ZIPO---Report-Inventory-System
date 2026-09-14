#pragma once

#include <QMainWindow>
#include <vector>
#include "../database/DatabaseManager.h"


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

        DatabaseManager database;
        void loadReports();
        void searchReports();
        void filterReports();
        void displayReports(const std::vector<Report>&reports);
        void applyTheme();
};