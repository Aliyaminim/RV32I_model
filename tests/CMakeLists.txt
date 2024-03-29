project(Testing)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(unit_test unit_test.cpp)
target_link_libraries(unit_test gtest gtest_main Threads::Threads)