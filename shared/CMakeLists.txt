# shared lib file

# get all .cpp files in src/
file(GLOB_RECURSE SHARED_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# this compiles as a static library using all .cpp files
add_library(${SHARED_LIB} STATIC
	${SHARED_SOURCES}
)

# make all headers publicly visible
target_include_directories(${SHARED_LIB} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)