# editor file

# get all .cpp files in src/
file(GLOB_RECURSE EDITOR_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# this compiles as an executable using all .cpp files
add_executable(${EDITOR_EXE}
	${EDITOR_SOURCES}
)

# include editor header files and shared/ so that the editor can see them
target_include_directories(${EDITOR_EXE} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/shared
)

# link both the engine and the shared lib
target_link_libraries(${EDITOR_EXE} PRIVATE
	${ENGINE_LIB}
	${SHARED_LIB}
)