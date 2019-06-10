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


#include "lidar.h"
#include <math.h>
#include "position.h"
#define DEG2RAD(x) ((x)*M_PI/180.)
#include "asserv.h"


using namespace std;
using namespace everest::hwdrivers;

C3iroboticsLidar robotics_lidar;

extern Asserv asserv;

float robot_width_thin = 750;
float robot_width_fat = 750;
float robot_lengh=450;

float angle_thin[4];
float angle_fat[4];
float tan_t[360];
float sin_t[360];
float cos_t[360];

float obstacle[4];
extern Position position;
float x_table_min=200;
float y_table_min=200;
float x_table_max=1800;
float y_table_max=2800;

bool CRC_error=false;
bool CRC_counter=0;

/*
void *run_lidar_watchdog(void *arg)
{
    if(robotics_lidar.CRC_wrong()==true)
    {
        CRC_counter++;
        if(CRC_counter>10000)
        {
            CRC_counter=0;
            CRC_error=true;
            pthread_join(asserv.lidar_thread, NULL);  
            if (pthread_create(&asserv.lidar_thread, NULL, run_lidar, NULL)) {
             perror("pthread_create");
        }
     }
    }
    usleep(500UL);
    
}
*/

void *run_lidar(void *arg)
{
	int    opt_com_baudrate = 230400;
    string opt_com_path = "/dev/ttyUSB0";

    CSerialConnection serial_connect;

    serial_connect.setBaud(opt_com_baudrate);
    serial_connect.setPort(opt_com_path.c_str());
    if(serial_connect.openSimple())
    {
        printf("[AuxCtrl] Open serail port sucessful!\n");
        printf("baud rate:%d\n",serial_connect.getBaud());
    }
    else
    {
        printf("[AuxCtrl] Open serail port %s failed! \n", opt_com_path.c_str());
        return NULL;
    }

    printf("C3iroboticslidar connected\n");

    robotics_lidar.initilize(&serial_connect);

    angle_thin[0]=atan(robot_lengh/robot_width_thin)*180/M_PI;
    angle_thin[1]=180-angle_thin[0];
    angle_thin[2]=angle_thin[0]+180;
    angle_thin[3]=angle_thin[1]+180;
    angle_fat[0]=atan(robot_lengh/robot_width_fat)*180/M_PI;
    angle_fat[1]=180-angle_fat[0];
    angle_fat[2]=angle_fat[0]+180;
    angle_fat[3]=angle_fat[1]+180;
    for( int i=0;i<360;i++)
       tan_t[i]=tan(DEG2RAD(i));
    for( int i=0;i<360;i++)
       sin_t[i]=sin(DEG2RAD(i));
    for( int i=0;i<360;i++)
       cos_t[i]=cos(DEG2RAD(i));
 

    while (CRC_error==false)
    {
		TLidarGrabResult result = robotics_lidar.getScanData();
        switch(result)
        {
            case LIDAR_GRAB_ING:
            {
//                printf("[Main] LIDAR_GRABING!\n");
                usleep(0UL);
                break;
            }
            case LIDAR_GRAB_SUCESS:
            {
                TLidarScan lidar_scan = robotics_lidar.getLidarScan();
                size_t lidar_scan_size = lidar_scan.getSize();
                std::vector<RslidarDataComplete> send_lidar_scan_data;
                send_lidar_scan_data.resize(lidar_scan_size);
                RslidarDataComplete one_lidar_data;
                obstacle[0]=8000;
                obstacle[1]=-8000;
                obstacle[2]=8000;
                obstacle[3]=-8000;
                float prev_value;
                for(size_t i = 0; i < lidar_scan_size; i++)
                {

                    prev_value=one_lidar_data.distance;

                    one_lidar_data.signal = lidar_scan.signal[i];
                    one_lidar_data.angle = 360-lidar_scan.angle[i];
                    one_lidar_data.distance = lidar_scan.distance[i];
                    send_lidar_scan_data[i] = one_lidar_data;
                    if(one_lidar_data.angle>89 && one_lidar_data.angle<91)
                        printf("lid log: %d\t %f\t %f \r\n",one_lidar_data.signal,one_lidar_data.angle,one_lidar_data.distance);
                
//                    if((one_lidar_data.signal>0) && ((prev_value-one_lidar_data.distance)>50 || (prev_value-one_lidar_data.distance)<-50))
                    {
//                        if(one_lidar_data.angle>89 && one_lidar_data.angle<91)
//                            printf("lid log: %d\t %f\t %f \r\n",one_lidar_data.signal,one_lidar_data.angle,one_lidar_data.distance);
                        
                        if((one_lidar_data.signal>0)&&(((one_lidar_data.angle>angle_fat[0])&&(one_lidar_data.angle<angle_fat[1]))||((one_lidar_data.angle>angle_fat[2])&&(one_lidar_data.angle<angle_fat[3]))))
                        {


                            int THETA=((int)(360+position.theta()*180/M_PI+one_lidar_data.angle))%360;
                            float X=position.x()+one_lidar_data.distance*1000*sin_t[THETA];
                            float Y=position.y()-one_lidar_data.distance*1000*cos_t[THETA];
//                                if(one_lidar_data.angle>89 && one_lidar_data.angle<91)
//                                   printf("ok:  %f\t %f\t %f\t %f\t %f\t %f\t %f\t\n",position.x(),position.y(),position.theta(),X,Y,one_lidar_data.angle,one_lidar_data.distance);
                            if((X>x_table_min)&&(X<x_table_max)&&(Y>y_table_min)&&(Y<y_table_max))
                            {
                                float x=one_lidar_data.distance*1000*cos_t[(int)(one_lidar_data.angle)];
                                float y=one_lidar_data.distance*1000*sin_t[(int)(one_lidar_data.angle)];
                                if((x<robot_width_thin/2)&&(x>-robot_width_thin/2))
                                {
                                    if(y>0)
                                    {
                                        if(y<obstacle[0])
                                            obstacle[0]=y;
                                    }
                                    else
                                    {
                                        if(y>obstacle[1])
                                            obstacle[1]=y;
                                    }
                                }
                                else if((x<robot_width_fat/2)&&(x>-robot_width_fat/2))
                                {
                                    if(y>0)
                                    {
                                        if(y<obstacle[2])
                                            obstacle[2]=y;
                                    }
                                    else
                                    {
                                        if(y>obstacle[3])
                                            obstacle[3]=y;
                                    }
                                }

                            }
                        }
                    }
                }

                printf("lidar %f\t%f %\t \n", obstacle[0],obstacle[1]);

	        usleep(50UL);

                break;
            }
            case LIDAR_GRAB_ERRO:
            {
                printf("[Main] LIDAR_GRAB_ERRO!\n");
        	usleep(50UL);
                break;
            }
            case LIDAR_GRAB_ELSE:
            {
                printf("[Main] LIDAR_GRAB_ELSE!\n");
	        usleep(50UL);
                break;
            }
        }
    }
    CRC_error=false;
    pthread_exit(NULL);

    return NULL;
}
