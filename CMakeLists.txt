cmake_minimum_required(VERSION 3.25)
project(opengl)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

include_directories(include)
link_directories(lib/x64)
add_executable(opengl final.cpp)
target_link_libraries(opengl libopengl32.a  libfreeglut.a  glu32.dll)