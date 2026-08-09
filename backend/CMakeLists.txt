cmake_minimum_required(VERSION 3.16)

project(DNP3 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin)

add_compile_options(
    -g
    -Wall
    -Wextra
)

# Logging Library
file(GLOB_RECURSE LOGGING_SRC
    CONFIGURE_DEPENDS
    libraries/logging/src/*.c
)

add_library(dnp3_logging STATIC
    ${LOGGING_SRC}
)

target_include_directories(dnp3_logging
    PUBLIC
        libraries/logging/inc
)

# Common Library
file(GLOB_RECURSE COMMON_SRC
    CONFIGURE_DEPENDS
    libraries/common/src/*.c
)

add_library(dnp3_common STATIC
    ${COMMON_SRC}
)

target_include_directories(dnp3_common
    PUBLIC
        libraries/common/inc
)

target_link_libraries(dnp3_common
    PRIVATE
        dnp3_parser
        dnp3_logging
)

# Network Library
file(GLOB_RECURSE NETWORK_SRC
    CONFIGURE_DPENDS
    libraries/network/src/*.c
)

add_library(dnp3_network STATIC
    ${NETWORK_SRC}
)

target_include_directories(dnp3_network
    PUBLIC
        libraries/network/inc
)

target_link_libraries(dnp3_network
    PRIVATE
        dnp3_logging
)

# Parser Library
file(GLOB_RECURSE PARSER_SRC
    CONFIGURE_DEPENDS
    libraries/parser/src/*.c
)

add_library(dnp3_parser STATIC
    ${PARSER_SRC}
)

target_include_directories(dnp3_parser
    PUBLIC
        libraries/parser/inc
)

target_link_libraries(dnp3_parser
    PRIVATE
        dnp3_logging
)

# Parser Executable
add_executable(parser
    parser-app/src/main.c
)

target_link_libraries(parser
    PRIVATE
        dnp3_logging
        dnp3_parser
)

# Client
file(GLOB_RECURSE CLIENT_SRC
    CONFIGURE_DEPENDS
    client/src/*.c
)

add_executable(client
    ${CLIENT_SRC}
)

target_include_directories(client PRIVATE
    client/inc
)

target_link_libraries(client
    PRIVATE
        dnp3_logging
        dnp3_parser
        dnp3_network
        dnp3_common
)

# Server
file(GLOB_RECURSE SERVER_SRC
    CONFIGURE_DEPENDS
    server/src/*.c
)

add_executable(server
    ${SERVER_SRC}
)

target_include_directories(server PRIVATE
    server/inc
)

target_link_libraries(server
    PRIVATE
        dnp3_logging
        dnp3_parser
        dnp3_network
        dnp3_common
)