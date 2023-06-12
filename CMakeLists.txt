cmake_minimum_required(VERSION 3.26)
project(tetris)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW 2.2 REQUIRED)

add_executable(tetris main.cpp model.cpp shaderprogram.cpp lodepng.cpp)

target_link_libraries(tetris glfw GL GLEW)
