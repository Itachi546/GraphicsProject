cmake_minimum_required(VERSION 3.0)
project(GraphicsProject)

SET(MSVC_FLAGS "/Zi /W3")
add_definitions(${MSVC_FLAGS})

include_directories(${PROJECT_SOURCE_DIR}/3rdParty/include)
include_directories(${PROJECT_SOURCE_DIR}/src/Math)
link_directories(${PROJECT_SOURCE_DIR}/3rdParty/lib)

add_executable(
	out
	src/Shader.cpp
	src/Input.cpp
	src/Grid.cpp
	src/Main.cpp
)

target_link_libraries(out SOIL glew32 opengl32 glfw3)
