cmake_minimum_required(VERSION 2.8.0)
project (asserv)
# Crée des variables avec les fichiers à compiler
set (CMAKE_CXX_STANDARD 11)

add_subdirectory("lidar")
file(GLOB SOURCES "lidar/src/src")

include_directories(
  lidar/src/include
)
set(SRCS
    "position.cpp"
    "parameter.cpp"
	"encoder.cpp"
	"control.cpp"
	"asserv.cpp"
	"servo.cpp"
	"lidar.cpp"
    )
    
set(HEADERS
    "position.h"
    "parameter.h"
    "encoder.h"
    "control.h"
    "asserv.h"
    "servo.h"
	"lidar.cpp"
    )

add_library(asserv ${SRCS} ${HEADERS})
TARGET_LINK_LIBRARIES (asserv lidar )
