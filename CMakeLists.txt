cmake_minimum_required(VERSION 3.13)

project(model_processor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
    message(FATAL_ERROR "In-source build is forbidden")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-D DEBUG -O0 -g -Wall -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(GTest REQUIRED)
enable_testing()
add_subdirectory(tests)

add_executable(rv32i_sim ./src/main.cpp)
target_include_directories(rv32i_sim PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(snippy_model)

#cmake -DCMAKE_BUILD_TYPE=Debug