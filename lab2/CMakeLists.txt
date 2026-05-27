cmake_minimum_required(VERSION 3.14)
project(lab2_CRC32)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

add_library(CRC32 INTERFACE)

target_include_directories(CRC32 INTERFACE 
src
)

find_package(GTest REQUIRED)

file(GLOB TEST_FILES CONFIGURE_DEPENDS "tests/*.cpp")
add_executable(tests ${TEST_FILES})
target_link_libraries(tests CRC32 GTest::gtest_main pthread)
