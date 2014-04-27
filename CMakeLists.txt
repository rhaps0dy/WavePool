cmake_minimum_required(VERSION 2.8)

project(WavePool)

#CXXFLAGS
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG -D_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g")
set(PRINT_CHANGES "NO" CACHE BOOL "Whether to print what changes have been
made when you change something in the application")

add_definitions("-std=c++11 -Wall -I/usr/local/include")

string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "" EMPTY_BUILD_TYPE)
if(EMPTY_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RELEASE")
endif(EMPTY_BUILD_TYPE)

if(PRINT_CHANGES)
	add_definitions("-DPRINT_CHANGES")
endif(PRINT_CHANGES)

# Find the SDL2 configuration file
set(SDL2_DIR ${CMAKE_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(SDL2 REQUIRED)

string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "RELEASE" IS_RELEASE)
string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" "RELWITHDEBINFO" IS_RWDI)
if(IS_RELEASE OR IS_RWDI)
	find_package(OpenMP)
	if (OPENMP_FOUND)
		set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	endif(OPENMP_FOUND)
endif(IS_RELEASE OR IS_RWDI)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS})

add_subdirectory(src)
