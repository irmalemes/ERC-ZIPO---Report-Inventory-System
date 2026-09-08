cmake_minimum_required(VERSION 3.20)

project(ReportInventorySystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Glavna aplikacija
add_executable(report_inventory
    src/main.cpp
    src/ReportInventory.cpp
    src/models/Report.cpp
    src/FileManager.cpp
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

# Qt/SQLite integration will be added in later milestones.