file(GLOB_RECURSE EDITOR_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${EDITOR}
	${EDITOR_SOURCES}
)

target_include_directories(${EDITOR} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/shared
)

target_link_libraries(${EDITOR} PRIVATE ${ENGINE})
target_link_libraries(${ENGINE} PRIVATE ${SHARED_LIB})