cmake_minimum_required(VERSION 3.20)

project(ReportInventorySystem LANGUAGES C CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sqlite3 STATIC
    libs/sqlite/sqlite3.c
)

target_include_directories(sqlite3 PUBLIC
    libs/sqlite
)

# Glavna aplikacija
add_executable(report_inventory
    src/main.cpp
    src/ReportInventory.cpp
    src/models/Report.cpp
    src/FileManager.cpp
    src/parser/ReportParser.cpp
    src/database/DatabaseManager.cpp
    src/gui/MainWindow.cpp
    src/upload/UploadManager.cpp
    src/calendar/ReportCalendar.cpp
)

target_link_libraries(report_inventory PRIVATE
    sqlite3
    Qt6::Widgets
)

# Milestone 02 testovi
add_executable(report_tests
    tests/ReportTests.cpp
    src/ReportInventory.cpp
    src/models/Report.cpp
)

# Milestone 03 testovi
add_executable(file_manager_tests
    tests/FileManagerTests.cpp
    src/FileManager.cpp
)

# Milestone 04 testovi
add_executable(report_parser_tests
    tests/ReportParserTests.cpp
    src/parser/ReportParser.cpp
    src/models/Report.cpp
)

add_executable(database_tests
    tests/DatabaseManagerTests.cpp
    src/database/DatabaseManager.cpp
    src/models/Report.cpp
)

target_link_libraries(database_tests PRIVATE sqlite3)

add_executable(upload_manager_tests
    tests/UploadManagerTests.cpp
    src/models/Report.cpp
    src/FileManager.cpp
    src/parser/ReportParser.cpp
    src/database/DatabaseManager.cpp
    src/upload/UploadManager.cpp
)

target_link_libraries(upload_manager_tests PRIVATE
    sqlite3
)

