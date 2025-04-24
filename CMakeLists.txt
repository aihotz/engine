# root file

cmake_minimum_required(VERSION 4.0.1)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

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

# include dependencies
include(dependencies.cmake)

# add shared/ engine/ and editor/ subdirectories
add_subdirectory(shared)
add_subdirectory(engine)
add_subdirectory(editor)

# building proof/ project
add_subdirectory(projects/proof)

# to organize inside visual studio explorer
set_target_properties(${SHARED_LIB} PROPERTIES FOLDER "engine")
set_target_properties(${ENGINE_LIB} PROPERTIES FOLDER "engine")
set_target_properties(${EDITOR_EXE} PROPERTIES FOLDER "engine")
set_target_properties(${GAME_LIB}   PROPERTIES FOLDER "engine")
set_target_properties(${GAME_EXE}   PROPERTIES FOLDER "engine")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${EDITOR_EXE})