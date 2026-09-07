cmake_minimum_required(VERSION 3.20)
project(ReportInventorySystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(report_inventory
    src/main.cpp
)

target_include_directories(report_inventory PRIVATE include)

# Qt/SQLite integration will be added in Milestone 5/6.
# Keep this initial project dependency-light so the intern can build the skeleton first.
