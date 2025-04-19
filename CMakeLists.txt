# root file

cmake_minimum_required(VERSION 4.0.1)

# project name
project(EngineWorkspace)

# c++ standard
set(CMAKE_CXX_STANDARD 23)

# set variables
set(ENGINE_LIB "engine")
set(EDITOR_EXE "editor")
set(SHARED_LIB "shared")
set(GAME_LIB "game_lib")
set(GAME_EXE "game_standalone")

# determine output directories depending on target (debug/release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/lib/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/lib/Release)

# add shared/ engine/ and editor/ subdirectories
add_subdirectory(shared)
add_subdirectory(engine)
add_subdirectory(editor)

# building proof/ project
add_subdirectory(projects/proof)