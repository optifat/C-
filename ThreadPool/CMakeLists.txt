cmake_minimum_required(VERSION 3.5)

project(ThreadPool)

set(CMAKE_CXX_STANDARD 11)

set(THREADS_PREFER_PTHREAD_FLAG ON)

set(SOURCE_FILES main.cpp ThreadPool.h Integral.h)
add_executable(ThreadPool ${SOURCE_FILES})

target_link_libraries(ThreadPool pthread)
