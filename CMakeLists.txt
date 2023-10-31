cmake_minimum_required(VERSION 3.10)
project(mandelbrot)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad)

find_package(OpenGL REQUIRED)

add_executable(mandelbrot src/main.cpp src/GLShader.cpp)
target_include_directories(mandelbrot PUBLIC lib/glad/include)
target_link_libraries(mandelbrot glfw OpenGL::GL glad)