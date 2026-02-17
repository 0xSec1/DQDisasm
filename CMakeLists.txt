cmake_minimum_required(VERSION 3.16)
project(DQDisasm VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)           #for neovim users :)

include_directories(${CMAKE_SOURCE_DIR})

find_path(CAPSTONE_INCLUDE_DIR capstone/capstone.h)
find_library(CAPSTONE_LIBRARY capstone)

if(NOT CAPSTONE_INCLUDE_DIR OR NOT CAPSTONE_LIBRARY)
    message(FATAL_ERROR "Capstone library not found. Install it")
endif()

set(SOURCES
    src/main.cpp
    src/ElfHandler.cpp
    src/Disassembler.cpp
)

add_executable(dqdisasm ${SOURCES})

target_include_directories(dqdisasm PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CAPSTONE_INCLUDE_DIR}
)

target_link_libraries(dqdisasm PRIVATE ${CAPSTONE_LIBRARY})
