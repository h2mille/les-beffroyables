cmake_minimum_required(VERSION 2.8.0)
project (robot)
add_subdirectory("asserv")
file(GLOB SOURCES "asserv/*.cpp")



find_package (Threads)

find_library(libname NAMES  libroboticscape.so PATHS ../Robotics_Cape_Installer/libraries)



add_executable(robot parser.cpp parser.h main.cpp)
TARGET_LINK_LIBRARIES(robot asserv ${libname})

target_link_libraries (robot ${CMAKE_THREAD_LIBS_INIT})


