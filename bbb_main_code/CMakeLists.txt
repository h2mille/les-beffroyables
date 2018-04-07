cmake_minimum_required(VERSION 3.5.0)
project (robot)
add_subdirectory("asserv")
file(GLOB SOURCES "asserv/*.cpp")

find_package (Threads)
find_library(lib_path NAMES roboticscape)
MESSAGE( STATUS "robotics cape path " ${lib_path})

add_executable(robot parser.cpp parser.h main.cpp)
TARGET_LINK_LIBRARIES (robot asserv ${lib_path})
TARGET_LINK_LIBRARIES (robot ${CMAKE_THREAD_LIBS_INIT})
