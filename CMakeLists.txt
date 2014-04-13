cmake_minimum_required(VERSION 2.8)

project(WavePool)

#CXXFLAGS
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
add_definitions("-std=c++11 -Wall -I/usr/local/include")

# Find the SDL2 configuration file
set(SDL2_DIR ${CMAKE_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SDL2 REQUIRED)

string(COMPARE EQUAL ${CMAKE_BUILD_TYPE} "RELEASE" IS_RELEASE)
if(IS_RELEASE)
	find_package(OpenMP)
	if (OPENMP_FOUND)
		set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	endif(OPENMP_FOUND)
endif(IS_RELEASE)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS})

add_subdirectory(src)
