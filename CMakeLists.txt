cmake_minimum_required(VERSION 3.20)

project(ReportInventorySystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(report_inventory
    src/main.cpp
    src/ReportInventory.cpp
    src/models/Report.cpp
)

add_executable(report_tests
    tests/ReportTests.cpp
    src/ReportInventory.cpp
    src/models/Report.cpp
)

# Qt/SQLite integration will be added in later milestones.
# Milestone 2 uses only standard C++.