cmake_minimum_required(VERSION 3.16)

project(arc_cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_<CONFIG> ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE  ${CMAKE_CURRENT_BINARY_DIR})

include_directories(../include)

file(GLOB SOURCES ../src/*.cpp)

add_executable(hwc_twoq_cache hwc_twoq_cache.cpp ${SOURCES})
add_executable(regression_tests regression_tests.cpp ${SOURCES})

add_subdirectory(unit_tests)


file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/test_data)
file(COPY test_data/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_data/)