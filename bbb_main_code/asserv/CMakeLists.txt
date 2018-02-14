cmake_minimum_required(VERSION 3.5.2)
project (asserv)
# Crée des variables avec les fichiers à compiler
set(SRCS
    "position.cpp"
    "parameter.cpp"
	"encoder.cpp"
	"control.cpp"
	"asserv.cpp"
    )
    
set(HEADERS
    "position.h"
    "parameter.h"
    "encoder.h"
    "control.h"
    "asserv.h"
    )

add_library(asserv ${SRCS} ${HEADERS})