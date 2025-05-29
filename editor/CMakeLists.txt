# editor file

# get all .cpp files in src/
file(GLOB_RECURSE EDITOR_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# this compiles as an executable using all .cpp files
add_executable(${EDITOR_EXE}
	${EDITOR_SOURCES}
)

target_compile_definitions(${EDITOR_EXE} PUBLIC EDITOR)

# include editor header files so that the editor can see them
target_include_directories(${EDITOR_EXE} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/sr)

# link both the engine and the shared lib
target_link_libraries(${EDITOR_EXE} PRIVATE
	${ENGINE_LIB}
	${SHARED_LIB}
)

# external dependencies
set(IMGUI_SOURCES
	${FETCHCONTENT_BASE_DIR}/imgui-src/backends/imgui_impl_sdl3.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/backends/imgui_impl_opengl3.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/imgui.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/imgui_demo.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/imgui_draw.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/imgui_tables.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/imgui_widgets.cpp
	${FETCHCONTENT_BASE_DIR}/imgui-src/misc/cpp/imgui_stdlib.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
	${FETCHCONTENT_BASE_DIR}/imgui-src
	${FETCHCONTENT_BASE_DIR}/imgui-src/backends
)

target_link_libraries(imgui PUBLIC SDL3::SDL3)

set(IMGUIZMO_SOURCES
	${FETCHCONTENT_BASE_DIR}/imguizmo-src/GraphEditor.cpp
	${FETCHCONTENT_BASE_DIR}/imguizmo-src/ImCurveEdit.cpp
	${FETCHCONTENT_BASE_DIR}/imguizmo-src/ImGradient.cpp
	${FETCHCONTENT_BASE_DIR}/imguizmo-src/ImGuizmo.cpp
	${FETCHCONTENT_BASE_DIR}/imguizmo-src/ImSequencer.cpp
)

add_library(imguizmo STATIC ${IMGUIZMO_SOURCES})
target_link_libraries(imguizmo PUBLIC imgui)

target_include_directories(imguizmo PUBLIC ${FETCHCONTENT_BASE_DIR}/imguizmo-src)

target_link_libraries(${EDITOR_EXE} PRIVATE 
	imgui
	imguizmo
	glbinding::glbinding
)