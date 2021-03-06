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
#include "parser.h"



using namespace std;



extern Position position;
extern Control control;
extern Asserv asserv;
extern parameter robot_parameter;
extern Parser parser;

//#ifdef USE_GYRO
    extern rc_imu_data_t data;
//#endif
void sigint_handler(int sig)
{
    /*do something*/
    printf("killing process %d\n",getpid());
    exit(0);
}
/*
int main(){
	signal(SIGINT, sigint_handler);
	rc_initialize();
#ifdef USE_GYRO
 	rc_imu_config_t conf = rc_default_imu_config();
	rc_initialize_imu(&data, conf);
#endif
	asserv.asserv_init();
	printf("handler configured \r\n");

	coordinates_t destination;
	destination.x = 0;
	destination.y = 0;
	destination.theta = 3*PI/2;
	coordinates_t add;
	add.x = 0;
	add.y = 100;
	add.theta = -PI/2;

	int i=0;
	
	
    
	while(rc_get_state() != EXITING)
	{
		asserv.add_coordinate(&destination,destination,add);
		printf("\r\nnext %d  %f %f %f\r\n",i++,destination.x, destination.y,destination.theta);
		asserv.go_destination(destination.x, destination.y, destination.theta);
		while(asserv.is_arrived()== false && rc_get_state() != EXITING)
					rc_usleep(100000000);		
	}
#ifdef USE_GYRO
	rc_power_off_imu();
#endif
	rc_cleanup();

}

*/

int main(){
	printf("\033[H\033[2J");

	signal(SIGINT, sigint_handler);
	rc_initialize();
//#ifdef USE_GYRO
 	rc_imu_config_t conf = rc_default_imu_config();
	rc_initialize_imu(&data, conf);
//#endif
	asserv.asserv_init();
	printf("handler configured \r\n");
	
	float distance;
	float ratio=47;
	
	parser.parser_init();
	
	coordinates_t destination;
	coordinates_t dot;
	
	// while(rc_get_state() != EXITING )
	// {

		// control.set_speed(left_t,100);
		// rc_usleep(1000000);
		// control.set_speed(left_t,800);
		// rc_usleep(1000000);
		
		
	// }
	

	// asserv.force_direction(force_forward);


	// asserv.add_move(-350,0,PI/2);
	// asserv.add_move(-700,0,PI/2);
	// asserv.add_move(-350,0,3*PI/2);

	// asserv.add_move(350,0,3*PI/2);
   	// asserv.add_move(700,0,PI/2);
	// asserv.add_move(350,0,3*PI/2);
	// asserv.add_move(0,0,PI/2);
	// asserv.go_destination();
	// asserv.go_angle(PI/2+2*PI);
	// asserv.add_move(0,-250,PI);
  	// asserv.add_move(0,-500,0);
	// asserv.add_move(0,-250,PI);
	// asserv.add_move(0,0,0);
	// asserv.add_move(0,250,PI);
  	// asserv.add_move(0,500,0);
	// asserv.add_move(0,250,PI);
	// asserv.add_move(0,0,0);
	// asserv.go_destination();


		
	
	int i=0;
	while(rc_get_state() != EXITING )
	{
//		printf("\r\nnext %d  %f %f %f\r\n",i++,destination.x, destination.y,destination.theta);
//		asserv.go_destination();
		usleep(100000UL);
		
	}
//#ifdef USE_GYRO
	rc_power_off_imu();
//#endif
    int fd;
    fd = open(myfifo_in,O_WRONLY);
    write(fd, "ko", strlen("ko")+1);
    close(fd);
    fd = open(myfifo_out,O_RDONLY);
    close(fd);
	rc_cleanup();

}

