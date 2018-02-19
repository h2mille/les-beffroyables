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


void sigint_handler(int sig)
{
    /*do something*/
    printf("killing process %d\n",getpid());
    exit(0);
}

int main(){
	signal(SIGINT, sigint_handler);
	rc_initialize();
	asserv.asserv_init();
	printf("handler configured \r\n");

	coordinates_t destination;
	destination.x = 0;
	destination.y = 0;
	destination.theta = PI/2;
	coordinates_t add;
	add.x = 0;
	add.y = 1000;
	add.theta = PI/2;
	int i=0;
    
    // control.set_speed(left_t , 10000);
	// control.run_wheel(left_t);
    // control.set_speed(right_t , 10000);
	// control.run_wheel(right_t);
	// while(rc_get_state() != EXITING);
	// rc_cleanup();
	// return 0;
	// asserv.add_coordinate(&destination,destination,add);	
    // asserv.go_destination(destination.x, destination.y, destination.theta);
	while(rc_get_state() != EXITING)
	{
		asserv.add_coordinate(&destination,destination,add);
		printf("\r\nnext %d  %f %f %f\r\n",i++,destination.x, destination.y,destination.theta);
		asserv.go_destination(destination.x, destination.y, destination.theta);
		
	}
	rc_cleanup();

}

