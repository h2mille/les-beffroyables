/*
*  3iRoboticsLIDAR System II
*  Driver Interface
*
*  Copyright 2017 3iRobotics
*  All rights reserved.
*
*	Author: 3iRobotics, Data:2017-09-15
*
*/


#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "lidar/src/include/C3iroboticsLidar.h"
#include "lidar/src/include/CSerialConnection.h"

#define DEG2RAD(x) ((x)*M_PI/180.)



typedef struct _rslidar_data
{
    _rslidar_data()
    {
        signal = 0;
        angle = 0.0;
        distance = 0.0;
    }
    uint8_t signal;
    float   angle;
    float   distance;
}RslidarDataComplete;

void *run_lidar_watchdog(void *arg);
void *run_lidar(void *arg);