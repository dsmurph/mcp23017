cmake_minimum_required(VERSION 3.10)

project(your_project VERSION 1.00 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/your_file.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
