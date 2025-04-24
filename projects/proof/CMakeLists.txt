# project file

# get all .cpp files in src/
file(GLOB_RECURSE GAME_LOGIC_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# this will compile both as a shared lib for the
# editor and as a standalone executable

### SHARED LIB ###
add_library(${GAME_LIB} SHARED
	${GAME_LOGIC_SOURCES}
)

# set the DLL_BUILD definition
target_compile_definitions(${GAME_LIB} PRIVATE DLL_BUILD)

# include project header files and shared/ so that the project can see them
target_include_directories(${GAME_LIB} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/shared
)

# link the engine and the shared lib
target_link_libraries(${GAME_LIB} PRIVATE 
	${SHARED_LIB}
	${ENGINE_LIB}
)

### EXECUTABLE ###
add_executable(${GAME_EXE}
	${GAME_LOGIC_SOURCES}
)

# include project header files and shared/ so that the project can see them
target_include_directories(${GAME_EXE} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/shared
)

# link the engine and the shared lib
target_link_libraries(${GAME_EXE} PRIVATE 
	${SHARED_LIB}
	${ENGINE_LIB}
)