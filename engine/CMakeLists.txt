file(GLOB_RECURSE ENGINE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(${ENGINE} STATIC
	${ENGINE_SOURCES}
)

target_include_directories(${ENGINE} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(PUBLIC_INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/src # temporarily whole src/, but will be refined in the future
)

target_include_directories(${ENGINE} PUBLIC
	${PUBLIC_INTERFACE}
	${CMAKE_SOURCE_DIR}/shared
)

target_link_libraries(${ENGINE} PRIVATE ${SHARED_LIB})