cmake_minimum_required(VERSION 3.16)

project(DNP3 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/bin)

# Parser Library
file(GLOB_RECURSE PARSER_SRC
    CONFIGURE_DEPENDS
    parser/src/*.c
)

add_library(dnp3_parser STATIC
    ${PARSER_SRC}
)

target_include_directories(dnp3_parser
    PUBLIC
        parser/inc
)

target_compile_options(dnp3_parser PRIVATE
    -g
)

# Parser Executable
add_executable(parser
    parser-app/src/main.c
)

target_link_libraries(parser
    PRIVATE
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
        dnp3_parser
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
        dnp3_parser
)