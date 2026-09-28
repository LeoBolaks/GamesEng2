cmake_minimum_required(VERSION 3.14)
project(lab03_calculator LANGUAGES CXX)

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

add_library(calculator STATIC src/calculator.cpp src/calculator.hpp)
target_include_directories(calculator PUBLIC src)

add_executable(lab03_tests test/lab03_tests.cpp)
target_link_libraries(lab03_tests PRIVATE calculator GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lab03_tests)
