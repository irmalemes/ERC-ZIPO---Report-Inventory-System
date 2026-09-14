#include "MainWindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QFont>


MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), database("data/report_inventory.db") {

    setWindowTitle("Report Inventory System");
    resize(1000, 600);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout -> setContentsMargins(20, 20, 20, 20);
    mainLayout -> setSpacing(12);

    //Naslov
    QLabel *titleLabel = new QLabel("Report Inventory System");
    QFont titleFont = titleLabel -> font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel -> setFont(titleFont);
    mainLayout -> addWidget(titleLabel);

    QLabel *subtitleLabel = new QLabel("Evidencija i pregled redovnih izvjestaja");
    QFont subtitleFont = subtitleLabel -> font();
    subtitleFont.setPointSize(10);
    subtitleLabel -> setFont(subtitleFont);
    mainLayout -> addWidget(subtitleLabel);



    //Pretraga, kategorija i dodavanje izvjestaja
    QHBoxLayout *controlLayout = new QHBoxLayout();

    searchLineEdit = new QLineEdit();
    searchLineEdit -> setPlaceholderText("Pretrazi izvjestaje...");
    connect(searchLineEdit, &QLineEdit::textChanged, this, &MainWindow::searchReports);

    categoryComboBox = new QComboBox();
    categoryComboBox -> addItem("Sve kategorije");
    categoryComboBox -> addItem("PIO/MIO");
    categoryComboBox -> addItem("CBBH");
    categoryComboBox -> addItem("HALCOM");
    connect(categoryComboBox, &QComboBox::currentTextChanged, this, &MainWindow::filterReports);

    themeButton = new QPushButton("Light");
    connect(themeButton, &QPushButton::clicked, this, [this](){
        darkTheme = !darkTheme;
        applyTheme();
    });


    addReportButton = new QPushButton("Dodaj izvjestaj");

    searchLineEdit -> setMinimumHeight(36);
    categoryComboBox -> setMinimumHeight(36);
    addReportButton -> setMinimumHeight(36);
    themeButton -> setMinimumHeight(36);

    categoryComboBox -> setMinimumWidth(150);
    addReportButton -> setMinimumWidth(130);
    themeButton -> setMinimumWidth(80);

    controlLayout -> addWidget(searchLineEdit);
    controlLayout -> addWidget(categoryComboBox);
    controlLayout -> addWidget(themeButton);
    controlLayout -> addWidget(addReportButton);

    mainLayout -> addLayout(controlLayout);

    //Tabela izvjestaja
    reportTable = new QTableWidget();
    reportTable -> setColumnCount(5);

    reportTable -> setHorizontalHeaderLabels({
        "Naziv fajla",
        "Kategorija",
        "Datum izvjestaja",
        "Velicina",
        "Putanja"
    });

    reportTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView::ResizeToContents);
    reportTable -> horizontalHeader() -> setStretchLastSection(true);
    reportTable -> setSelectionBehavior(QAbstractItemView::SelectRows);
    reportTable -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    reportTable -> setAlternatingRowColors(true);



    mainLayout -> addWidget(reportTable);

    reportCountLabel = new QLabel("Prikazano izvjestaja: 0");
    mainLayout -> addWidget(reportCountLabel);

    applyTheme();

    //baza podataka
    if(database.open()){
        database.createReportsTable();
        loadReports();
    }
}


void MainWindow::loadReports(){

    std::vector<Report>reports = database.getAllReports();

    displayReports(reports);
    
}


void MainWindow::searchReports(){

    QString searchText = searchLineEdit -> text();
    
    if(searchText.isEmpty()){
        loadReports();
        return;
    }

    std::vector<Report>reports = database.searchByFilename(searchText.toStdString());

    displayReports(reports);
}


void MainWindow::filterReports(){

    QString selectedCategory = categoryComboBox -> currentText();

    if(selectedCategory == "Sve kategorije"){
        loadReports();
        return;
    } 

    ReportCategory category;

    if(selectedCategory == "PIO/MIO"){
        category = ReportCategory::PIO_MIO;
    }

    else if(selectedCategory == "CBBH"){
        category = ReportCategory::CBBH;
    }

    else {
        category = ReportCategory::HALCOM;
    }

    std::vector<Report>reports = database.filterByCategory(category);

    displayReports(reports);
}


void MainWindow::displayReports(const std::vector<Report>&reports){

    reportTable -> setRowCount(0);

    for(const Report &report : reports){
        int row = reportTable -> rowCount();
        reportTable -> insertRow(row);

        reportTable -> setItem(row, 0, new QTableWidgetItem(QString::fromStdString(report.getFilename())));

        QString categoryText;

        switch(report.getCategory()){
            case ReportCategory::PIO_MIO:
                categoryText = "PIO/MIO";
                break;

            case ReportCategory::CBBH:
                categoryText = "CBBH";
                break;

            case ReportCategory::HALCOM:
                categoryText = "HALCOM";
                break;
        }

        reportTable -> setItem(row, 1, new QTableWidgetItem(categoryText));
        reportTable -> setItem(row, 2, new QTableWidgetItem(QString::fromStdString(report.getReportDate())));
        reportTable -> setItem(row, 3, new QTableWidgetItem(QString::number(report.getFileSize())));
        reportTable -> setItem(row, 4, new QTableWidgetItem(QString::fromStdString(report.getFilePath())));
    }

    reportCountLabel -> setText("Prikazano izvjestaja: " + QString::number(reports.size()));
}

void MainWindow::applyTheme(){

    if(darkTheme){

        themeButton -> setText("Light");

        setStyleSheet(R"(
            QMainWindow {
                background-color: #1e1e1e;
            }

            QLabel {
                color: #f0f0f0;
            }

            QLineEdit, QComboBox {
                background-color: #2b2b2b;
                color: #f0f0f0;
                border: 1px solid #555555;
                border-radius: 5px;
                padding: 6px;
                selection-background-color: #3b82f6;
                selection-color: white;
            }

            QComboBox QAbstractItemView {
                background-color: #2b2b2b;
                color: #f0f0f0;
                selection-background-color: #3b82f6;
                selection-color: white;
            }

            QPushButton {
                background-color: #3b82f6;
                color: white;
                border: none;
                border-radius: 5px;
                padding: 6px 12px;
            }

            QPushButton:hover {
                background-color: #2563eb;
            }

            QTableWidget {
                background-color: #252525;
                alternate-background-color: #2d2d2d;
                color: #f0f0f0;
                gridline-color: #444444;
                border: 1px solid #444444;
                selection-background-color: #3b82f6;
                selection-color: white;
            }

            QHeaderView::section {
                background-color: #333333;
                color: #f0f0f0;
                padding: 6px;
                border: 1px solid #444444;
            }
        )");
    }

    else {

        themeButton -> setText("Dark");

        setStyleSheet(R"(
            QMainWindow {
                background-color: #f5f5f5;
            }

            QLabel {
                color: #202020;
            }

            QLineEdit, QComboBox {
                background-color: white;
                color: #202020;
                border: 1px solid #cccccc;
                border-radius: 5px;
                padding: 6px;
                selection-background-color: #3b82f6;
                selection-color: white;
            }

            QComboBox QAbstractItemView {
                background-color: white;
                color: #202020;
                selection-background-color: #3b82f6;
                selection-color: white;
            }

            QPushButton {
                background-color: #3b82f6;
                color: white;
                border: none;
                border-radius: 5px;
                padding: 6px 12px;
            }

            QPushButton:hover {
                background-color: #2563eb;
            }

            QTableWidget {
                background-color: white;
                alternate-background-color: #f2f2f2;
                color: #202020;
                gridline-color: #dddddd;
                border: 1px solid #cccccc;
                selection-background-color: #3b82f6;
                selection-color: white;
            }

            QHeaderView::section {
                background-color: #e9e9e9;
                color: #202020;
                padding: 6px;
                border: 1px solid #cccccc;
            }
        )");
    }
}