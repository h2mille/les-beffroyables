cmake_minimum_required(VERSION 2.8.0)
project (asserv)
# Crée des variables avec les fichiers à compiler

set (CMAKE_CXX_STANDARD 11)

set(SRCS
    "position.cpp"
    "parameter.cpp"
	"encoder.cpp"
	"control.cpp"
	"asserv.cpp"
	"servo.cpp"
    )
    
set(HEADERS
    "position.h"
    "parameter.h"
    "encoder.h"
    "control.h"
    "asserv.h"
    "servo.h"
    )

add_library(asserv ${SRCS} ${HEADERS})
