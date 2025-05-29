# engine file

# get all .cpp files in src/
file(GLOB_RECURSE ENGINE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# this compiles as a static library using all .cpp files
add_library(${ENGINE_LIB} STATIC
	${ENGINE_SOURCES}
)

# include engine header files so that the engine can see them
target_include_directories(${ENGINE_LIB} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/src/engine
)

# stablish which headers are publicly visible through the engine/ directory
set(PUBLIC_INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/src # temporarily whole src/, but will be refined in the future
)

# make the public interface visible
target_include_directories(${ENGINE_LIB} PUBLIC
	${PUBLIC_INTERFACE}
)

# link the shared lib
target_link_libraries(${ENGINE_LIB} PRIVATE ${SHARED_LIB})

# external dependencies
target_link_libraries(${ENGINE_LIB} PRIVATE
	SDL3::SDL3
	glbinding::glbinding
	freetype
)