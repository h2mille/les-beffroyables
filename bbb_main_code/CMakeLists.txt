cmake_minimum_required(VERSION 3.5.2)
project (robot)
add_subdirectory("asserv")
file(GLOB SOURCES "asserv/*.cpp")




find_package (Threads)
find_library(roboticscape roboticscape libraries)
message("roboticscape is at: ${roboticslib}")


add_executable(robot main.cpp)
TARGET_LINK_LIBRARIES(robot asserv )
TARGET_LINK_LIBRARIES(robot roboticscape)
target_link_libraries (robot ${CMAKE_THREAD_LIBS_INIT})


