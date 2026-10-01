cmake_minimum_required(VERSION 3.14)
project(lab04_region_scan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(region_scan STATIC src/region_scan.cpp src/region_scan.hpp)
target_include_directories(region_scan PUBLIC src)

add_executable(lab04_tests test/lab04_tests.cpp)
target_link_libraries(lab04_tests PRIVATE region_scan GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lab04_tests)
