cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CPP_COMPILER "g++")

project("shroomy")


# Files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")
set(BLOB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/blob")


# Include Lua sources
file(READ "${SRC_DIR}/lua/lua.lua" LUA_LUA)
configure_file("${SRC_DIR}/internal_lua/internal_lua_files.h.in" "${BLOB_DIR}/internal_lua_files.h" @ONLY)

# Output
file(GLOB_RECURSE SRCS "${SRC_DIR}/*.cpp")
add_executable(shroomy ${SRCS})

if(DEBUG)
    #target_compile_definitions(shroomy PRIVATE SHROOMY_DEBUG)
    message("WARNING: Making debug version")
    target_compile_options(shroomy PRIVATE
        -Wall -Wextra
        -static
        -O0
    )
else()
    #target_compile_definitions(shroomy PRIVATE SHROOMY_BUILD)
    target_compile_options(shroomy PRIVATE
        -static
        -Ofast
    )
endif()

# Compile flags
target_compile_options(shroomy PRIVATE "-std=c++20")

# Includs
target_include_directories(shroomy PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/luajit/src")
target_include_directories(shroomy PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(shroomy PRIVATE ${BLOB_DIR})

# Libraries
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

target_link_libraries(shroomy SDL2::SDL2)
target_link_libraries(shroomy SDL2_image::SDL2_image)
target_link_libraries(shroomy SDL2_mixer::SDL2_mixer)
target_link_libraries(shroomy "${LIBS_DIR}/luajit/src/libluajit.so")