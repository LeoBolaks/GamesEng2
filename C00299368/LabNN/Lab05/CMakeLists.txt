cmake_minimum_required(VERSION 3.14)
project(lab05_machine LANGUAGES CXX)

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

add_library(machine STATIC src/machine.cpp src/machine.hpp)
target_include_directories(machine PUBLIC src)

add_executable(lab05_tests test/lab05_tests.cpp)
target_link_libraries(lab05_tests PRIVATE machine GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lab05_tests)
