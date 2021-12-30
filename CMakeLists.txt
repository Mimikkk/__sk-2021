cmake_minimum_required(VERSION 3.22)
project(sk_chat_project C)

set(CMAKE_C_STANDARD 23)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

# Source directories and files
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src/backend)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

# Executable output directory
set(EXECUTABLE_OUTPUT_PATH ${BIN_DIR})

# All needed source files
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.h ${SRC_DIR}/*.c)
message(STATUS "Found ${SOURCES} source files")

add_executable(server ${SRC_DIR}/server.c ${SOURCES})
add_executable(client ${SRC_DIR}/server.c ${SOURCES})
target_include_directories(server PRIVATE ${SRC_DIR})
target_include_directories(client PRIVATE ${SRC_DIR})
