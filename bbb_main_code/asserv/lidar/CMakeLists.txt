cmake_minimum_required(VERSION 2.8.3)
project(lidar)

set(DELTA_2B_LIDAR_SDK_PATH "./src/")

FILE(GLOB DELTA_2B_LIDAR_SDK_SRC 
  "${DELTA_2B_LIDAR_SDK_PATH}/src/*.cpp"
)
FILE(GLOB DELTA_2B_LIDAR_SDK_INC
  "${DELTA_2B_LIDAR_SDK_PATH}/include/*.h"
)

include_directories(
  ${DELTA_2B_LIDAR_SDK_PATH}/include
  ${DELTA_2B_LIDAR_SDK_PATH}/src
)

add_library(lidar ${DELTA_2B_LIDAR_SDK_SRC} ${DELTA_2B_LIDAR_SDK_INC})
target_link_libraries(lidar -lrt -lpthread)


