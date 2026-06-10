cmake_minimum_required(VERSION 3.10)
project(dnp3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Put the runtime executable into the build/bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Collect source files from src/
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_executable(dnp3 ${SRCS})

target_include_directories(dnp3 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(dnp3 PRIVATE -Wall -Wextra -O2 -g)

install(TARGETS dnp3 RUNTIME DESTINATION bin)