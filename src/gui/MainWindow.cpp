#include "MainWindow.h"

#include <filesystem>
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
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QDate>
#include <QColor>


MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), database("data/report_inventory.db"), uploadManager(database) {

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


    reminderTitleLabel = new QLabel("PODSJETNICI");
    QFont reminderTitleFont = reminderTitleLabel -> font();
    reminderTitleFont.setPointSize(12);
    reminderTitleFont.setBold(true);
    reminderTitleLabel -> setFont(reminderTitleFont);
    reminderDeadlineLabel = new QLabel("Sljedeci rok: -");
    reminderStatusLabel = new QLabel("Status trenutnog perioda: -");
    reminderMessageLabel = new QLabel("");
    cbbhReminderLabel = new QLabel("");
    QFont cbbhFont = cbbhReminderLabel -> font();
    cbbhFont.setBold(true);
    cbbhReminderLabel -> setFont(cbbhFont);
    cbbhReminderLabel -> setStyleSheet("color: orange; font-weight: bold;");



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


    periodComboBox = new QComboBox();
    periodComboBox -> addItem("01 - 15");
    periodComboBox -> addItem("16 - kraj mjeseca");

    monthComboBox = new QComboBox();
    monthComboBox -> addItems({
        "Januar",
        "Februar",
        "Mart",
        "April",
        "Maj",
        "Juni",
        "Juli",
        "August",
        "Septembar",
        "Oktobar",
        "Novembar",
        "Decembar"
    });

    monthComboBox -> setCurrentIndex(QDate::currentDate().month()-1);

    periodOverviewButton = new QPushButton("Pregled perioda");
    connect(periodOverviewButton, &QPushButton::clicked, this, &MainWindow::showPeriodOverview);


    yearSpinBox = new QSpinBox();
    yearSpinBox -> setRange(2020, 2100);
    yearSpinBox -> setValue(QDate::currentDate().year());

    themeButton = new QPushButton("Light");
    connect(themeButton, &QPushButton::clicked, this, [this](){
        darkTheme = !darkTheme;
        applyTheme();
    });


    addReportButton = new QPushButton("Dodaj izvjestaj");
    connect(addReportButton, &QPushButton::clicked, this, &MainWindow::addReport);

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
    controlLayout -> addWidget(periodComboBox);
    controlLayout -> addWidget(monthComboBox);
    controlLayout -> addWidget(yearSpinBox);
    controlLayout -> addWidget(periodOverviewButton);

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


    periodTable = new QTableWidget();
    periodTable -> setColumnCount(3);
    periodTable -> setHorizontalHeaderLabels({"Kategorija", "Tip izvjestaja", "Status"});
    periodTable -> horizontalHeader() -> setSectionResizeMode(QHeaderView::ResizeToContents);
    periodTable -> horizontalHeader() -> setStretchLastSection(true);
    periodTable -> setEditTriggers(QAbstractItemView::NoEditTriggers);
    periodTable -> setSelectionBehavior(QAbstractItemView::SelectRows);
    periodTable -> setAlternatingRowColors(true);


    periodSummaryLabel = new QLabel("Odaberi period i klikni Pregled perioda.");


    reportCountLabel = new QLabel("Prikazano izvjestaja: 0");
    mainLayout -> addWidget(reportCountLabel);
    mainLayout -> addWidget(periodSummaryLabel);
    mainLayout -> addWidget(periodTable);

    //Reminder panel
    mainLayout -> addWidget(reminderTitleLabel);
    mainLayout -> addWidget(reminderDeadlineLabel);
    mainLayout -> addWidget(reminderStatusLabel);
    mainLayout -> addWidget(reminderMessageLabel);
    mainLayout -> addWidget(cbbhReminderLabel);


    applyTheme();

    //baza podataka
    if(database.open()){
        database.createReportsTable();
        loadReports();
    }

    updateReminderPanel();
}


void MainWindow::updateReminderPanel() {
    QDate currentDate = QDate::currentDate();

    int day = currentDate.day();
    int month = currentDate.month();
    int year = currentDate.year();
    int daysInMonth = currentDate.daysInMonth();

    reminderTitleLabel -> setText("PODSJETNICI - " + currentDate.toString("dd.MM.yyyy"));

    ReportPeriod currentPeriod = reportCalendar.getReportPeriod(day);
    std::vector<Report>reports = database.getAllReports();
    ReportPeriodResult periodResult = reportCalendar.analyzePeriod(reports, currentPeriod, month, year);

    ReminderResult reminder = reminderManager.createReminder(day, month, year, daysInMonth, periodResult);

    reminderDeadlineLabel -> setText(
        "Sljedeci rok: " +
        QString::fromStdString(reminder.nextDeadline) +
        " | Preostalo dana: " +
        QString::number(reminder.daysRemaining)
    );

    reminderStatusLabel -> setText(
        "Status trenutnog perioda: " +
        QString::fromStdString(
            reportCalendar.getStatusName(reminder.reportStatus)
        ) +
        " | Nedostaje: " +
        QString::number(reminder.missingReports)
    );

    reminderMessageLabel -> setText(QString::fromStdString(reminder.message));

    if(reminder.deadlinePassed && reminder.missingReports > 0){
        reminderMessageLabel -> setStyleSheet(
            "color: red; font-weight: bold;"
        );
    }
    else{
        reminderMessageLabel -> setStyleSheet("");
    }

    cbbhReminderLabel -> setText(QString::fromStdString(reminder.cbbhMessage));






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


void MainWindow::addReport(){

    QString filePath = QFileDialog::getOpenFileName(this, "Odaberi izvjestaj", "", "Izvjestaji (*.pdf *.docx)");

    if(filePath.isEmpty()){
        return;
    }

    if(!uploadManager.isValidFile(filePath.toStdString())) {

        QMessageBox::warning(this, "Neispravan fajl", "Odabrani fajl nije validan PDF ili DOCX dokument!");

        return;
    }

    std::optional<Report>parsedReport = uploadManager.parseReport(filePath.toStdString());

    if(!parsedReport.has_value()) {

        QStringList categories = { "PIO/MIO", "CBBH", "HALCOM" };
        bool ok;

        QString selectedCategory = QInputDialog::getItem(this, "Odabir kategorije", "Naziv izvjestaja nije prepoznat.\n"
            "Odaberi kategoriju:", categories, 0, false, &ok);

        
        if(!ok) {
            return;
        }

        QStringList reportTypes;

        if(selectedCategory == "PIO/MIO"){
            
            reportTypes = { "FZ MIOPIO SARAJEVO", "FZ MIOPIO MOSTAR"};

        }

        else if(selectedCategory == "CBBH") {

            reportTypes = { "ZAPISNIK BPRV"};

        }

        else if(selectedCategory == "HALCOM") {

            reportTypes = { "HALCOM REPORT", "CBBH INFRASTRUCTURE REPORT"};
        }

        QString selectedType = QInputDialog::getItem(this, "Odabir tipa", "Odaberi tip izvjestaja:", reportTypes, 0, false, &ok);

        if(!ok){
            return;
        }

        QString reportDate = QInputDialog::getText(this, "Datum izvjestaja", "Unesi datum izvjestaja (dd.mm.yyyy):", QLineEdit::Normal, "", &ok);
        
        if(!ok || reportDate.isEmpty()) {
            return;
        }

        QDate validDate = QDate::fromString(reportDate, "dd.MM.yyyy");

        if(!validDate.isValid()) {
            QMessageBox::warning(this, "Neispravan datum", "Datum mora biti ispravan i u formatu dd.mm.yyyy.");
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

        ReportType reportType;

        if(selectedType == "FZ MIOPIO SARAJEVO"){
            reportType = ReportType::FZ_MIOPIO_SARAJEVO;
        }

        else if(selectedType == "FZ MIOPIO MOSTAR"){
            reportType = ReportType::FZ_MIOPIO_MOSTAR;
        }

        else if(selectedType == "ZAPISNIK BPRV"){
            reportType = ReportType::ZAPISNIK_BPRV;
        }

        else if(selectedType == "HALCOM REPORT"){
            reportType = ReportType::HALCOM_REPORT;
        }

        else {
            reportType = ReportType::CBBH_INFRASTRUCTURE_REPORT;
        }

        std::string filename = std::filesystem::path(filePath.toStdString()).filename().string();
        std::string uploadDate = QDate::currentDate().toString("dd.MM.yyyy").toStdString();

        Report manualReport(filename, category, reportType, reportDate.toStdString(), uploadDate, 0, filePath.toStdString());
        parsedReport = manualReport;
    }

    Report report = parsedReport.value();
    report.setFilePath(filePath.toStdString());

    if(!uploadManager.uploadReport(report)) {

        QMessageBox::warning(this, "Greska", "Izvjestaj nije moguce dodati. Moguce je da izvjestaj vec postoji.");
        return;
    }

    loadReports();
    updateReminderPanel();
    QMessageBox::information(this, "Uspjesan upload", "Izvjestaj je uspjesno dodan");


}

void MainWindow::showPeriodOverview() {
    
    int month = monthComboBox -> currentIndex() + 1;
    int year = yearSpinBox -> value();

    ReportPeriod period;

    if(periodComboBox -> currentIndex() == 0) {
        period = ReportPeriod::FIRST_HALF;
    } else {
        period = ReportPeriod::SECOND_HALF;
    }

    std::vector<Report>reports = database.getAllReports();
    ReportPeriodResult result = reportCalendar.analyzePeriod(reports, period, month, year);

    QString summaryText = 
        "Ocekivano: " + QString::number(result.expectedReports) +
        " | Primljeno: " + QString::number(result.receivedReports) + 
        " | Nedostaje: " + QString::number(result.missingReports) +
        " | Zavrseno: " + QString::number(result.completionPercentage, 'f', 0) + "%" +
        " | Status: " + QString::fromStdString(
                reportCalendar.getStatusName(result.status)
        );


    periodSummaryLabel -> setText(summaryText);


    std::vector<ReportType>expectedTypes = reportCalendar.getExpectedReportTypes();
    periodTable -> setRowCount(expectedTypes.size());

    for(int i = 0; i < expectedTypes.size(); i++) {

        ReportType reportType = expectedTypes[i];
        ReportCategory category = reportCalendar.getCategoryForType(reportType);
        bool missing = false;

        for(ReportType missingType : result.missingReportTypes) {

            if(reportType == missingType) {
                missing = true;
                break;
            }
        }


        QString status;
        if(missing) {
            status = "MISSING";
        } else {
            status = "RECEIVED";
        }


        periodTable -> setItem(i, 0, new QTableWidgetItem(QString::fromStdString(reportCalendar.getCategoryName(category))));

        periodTable -> setItem(i, 1, new QTableWidgetItem(QString::fromStdString(reportCalendar.getReportTypeName(reportType))));

        QTableWidgetItem *statusItem = new QTableWidgetItem(status);

        if(missing) {
            statusItem -> setForeground(QColor("red"));
        } else {
            statusItem -> setForeground(QColor("green"));
        }

        periodTable -> setItem(i, 2, statusItem);

    




    }



    QString overview;
    overview += "Ocekivano: " + QString::number(result.expectedReports) + "\n";
    overview += "Primljeno: " + QString::number(result.receivedReports) + "\n";
    overview += "Nedostaje: " + QString::number(result.missingReports) + "\n";
    overview += "Zavrseno: " + QString::number(result.completionPercentage, 'f', 0) + "%\n";
    overview += "Status: " + QString::fromStdString(reportCalendar.getStatusName(result.status));

    if(!result.missingReportTypes.empty()) {
        
        overview += "\n\nNedostajuci izvjestaji:\n";

        for(ReportType reportType : result.missingReportTypes) {
            ReportCategory category = reportCalendar.getCategoryForType(reportType);
            overview += "- " + QString::fromStdString(reportCalendar.getCategoryName(category)) + " | " + QString::fromStdString(reportCalendar.getReportTypeName(reportType)) + "\n";     
        }
    }

    QMessageBox::information(this, "Pregled perioda", overview);


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