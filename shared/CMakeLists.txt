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

# external dependencies
target_include_directories(${SHARED_LIB} PRIVATE
    ${FETCHCONTENT_BASE_DIR}/stb-src/
    ${FETCHCONTENT_BASE_DIR}/tinyobjloader-src/
)

target_include_directories(${SHARED_LIB} PUBLIC
    ${FETCHCONTENT_BASE_DIR}/glm-src/
    ${FETCHCONTENT_BASE_DIR}/json-src/include
)