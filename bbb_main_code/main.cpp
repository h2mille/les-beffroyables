#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "asserv/encoder.h"
#include "asserv/position.h"
#include "asserv/control.h"
#include "asserv/parameter.h"
#include "asserv/asserv.h"
#include "rc_usefulincludes.h"
#include <pthread.h>
#include "roboticscape.h"
#include "asserv/types.h"



using namespace std;



extern Position position;
extern Control control;
extern Asserv asserv;
extern parameter robot_parameter;

#ifdef USE_GYRO
    extern rc_imu_data_t data;
#endif
void sigint_handler(int sig)
{
    /*do something*/
    printf("killing process %d\n",getpid());
    exit(0);
}

// int main(){
	// signal(SIGINT, sigint_handler);
	// rc_initialize();
// #ifdef USE_GYRO
 	// rc_imu_config_t conf = rc_default_imu_config();
	// rc_initialize_imu(&data, conf);
// #endif
	// asserv.asserv_init();
	// printf("handler configured \r\n");

	// coordinates_t destination;
	// destination.x = 0;
	// destination.y = 0;
	// destination.theta = PI/2;
	// coordinates_t add;
	// add.x = 0;
	// add.y = 100;
	// add.theta = -PI/2;

	// int i=0;
    
	// while(rc_get_state() != EXITING)
	// {
		// asserv.add_coordinate(&destination,destination,add);
		// printf("\r\nnext %d  %f %f %f\r\n",i++,destination.x, destination.y,destination.theta);
		// asserv.go_destination(destination.x, destination.y, destination.theta);
		
	// }
// #ifdef USE_GYRO
	// rc_power_off_imu();
// #endif
	// rc_cleanup();

// }

int main(){
	printf("\033[H\033[2J");

	signal(SIGINT, sigint_handler);
	rc_initialize();
#ifdef USE_GYRO
 	rc_imu_config_t conf = rc_default_imu_config();
	rc_initialize_imu(&data, conf);
#endif
	asserv.asserv_init();
	printf("handler configured \r\n");
	
	float distance;
	float ratio=47;
	
	coordinates_t destination;
	// destination.x = 10;
	// destination.y = -100;
	// destination.theta = 3*PI/2;
	
	// control.distance_direct_arc( destination, &distance, &ratio,asserv_any_dir);
    // control.distance_direct_arc( destination, &distance, &ratio,force_frontward);
	// printf("result: %f %f\r\n",distance, ratio);
	// rc_cleanup();
	// return 0;

	destination.x = 0;
	destination.y = 0;
	destination.theta = PI/2;
	coordinates_t add;
	add.x = 0;
	add.y = 250;
	add.theta = PI/2;
	int i=0;
	printf("dest liste\r\n");
	for(i=0;i<4;i++)
	{
		asserv.add_coordinate(&destination,destination,add);
		asserv.add_move(destination.x,destination.y,destination.theta);
		printf("%f\t%f\t%f\r\n",destination.x,destination.y,destination.theta);
	}
	// for(i=0;i<4;i++)
	// {
		// coordinates_t pos;
		// float dist;
		// asserv.get_next_move(&pos,&dist);
		// printf("next pos %f %f %f %f\r\n",pos.x,pos.y,pos.theta,dist);
		// asserv.go_next();
	// }
	float dist;
	coordinates_t pos;
	while(rc_get_state() != EXITING )
	{
		printf("\r\nnext %d  %f %f %f\r\n",i++,destination.x, destination.y,destination.theta);
		asserv.go_destination();
		
	}
#ifdef USE_GYRO
	rc_power_off_imu();
#endif
	rc_cleanup();

}

