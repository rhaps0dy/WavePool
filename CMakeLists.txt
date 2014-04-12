cmake_minimum_required(VERSION 2.8)

project(Infografia2014)

# Troba el fitxer de configuracio de SDL2
set(SDL2_DIR ${CMAKE_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS})

add_subdirectory(src)
