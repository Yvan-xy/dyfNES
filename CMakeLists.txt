cmake_minimum_required(VERSION 3.15)
project(dyfNES)

set(CMAKE_CXX_STANDARD 14)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (CPU_LOG "1")
include_directories(${PROJECT_INCLUDE_DIR})

message(STATUS "source -> ${PROJECT_SOURCE_DIR}/src")
message(STATUS "include -> ${PROJECT_INCLUDE_DIR}")

configure_file(
        "${PROJECT_INCLUDE_DIR}/config.h.in"
        "${PROJECT_INCLUDE_DIR}/config.h"
)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

message(STATUS "source -> ${SOURCES}")
add_executable(dyfNES main.cpp ${SOURCES})
target_link_libraries(dyfNES)
