cmake_minimum_required(VERSION 2.8)

project(WavePool)

#CXXFLAGS
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
add_definitions("-std=c++11 -DDOUBLE_PRECISION")

# Find the SDL2 configuration file
set(SDL2_DIR ${CMAKE_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

add_subdirectory(src)
