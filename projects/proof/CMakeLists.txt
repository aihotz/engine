file(GLOB_RECURSE GAME_LOGIC_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(${GAME} SHARED
	${GAME_LOGIC_SOURCES}
)

target_include_directories(${GAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/shared
)

target_link_libraries(${ENGINE} PRIVATE ${SHARED_LIB})