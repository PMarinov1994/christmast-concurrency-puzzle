cmake_minimum_required(VERSION 3.10)

project(concurrency C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

FILE(GLOB SOURCES *.c)

add_executable(concurrency ${SOURCES})

target_link_libraries(concurrency PRIVATE Threads::Threads)
