project(arc_cache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

include_directories(../include)

file(GLOB SOURCES ../src/*.cpp)

add_executable(arc_cache_runner twoq_cache_runner.cpp ${SOURCES})
