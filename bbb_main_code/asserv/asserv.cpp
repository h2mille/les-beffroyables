#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "encoder.h"
#include "position.h"
#include "control.h"
#include "parameter.h"
#include "asserv.h"
#include "rc_usefulincludes.h"
#include <pthread.h>


using namespace std;

#include "roboticscape.h"
#include "types.h"

Asserv asserv;

asserv_type_t Asserv::get_asserv_mode(){
	return asserv_mode;
}

bool Asserv::is_arrived(){
	return arrived;
}
void Asserv::set_asserv_mode(asserv_type_t asserv){
	asserv_mode=asserv;
}

void Asserv::set_arrived(bool isit){
	arrived=isit;
}



static void *position_computation(void *arg){ 
	while(rc_get_state() != EXITING){
		 position.Update_coordinates();
		 printf("\r");
		 position.print_coordinates();
		fflush(stdout);
		rc_usleep(robot_parameter.period());

	}
	pthread_exit(NULL);
		
}

static void *control_wheels(void *arg){
	while(rc_get_state() != EXITING){
		float dist;
		float ratio;
		switch(asserv.get_asserv_mode())
		{
			case(asserv_vector):
				control.distance_arc(position.get_coordinates(),control.get_destination(),&dist,&ratio);
				printf("distance %f %f",dist,ratio);
				if(dist*dist <5	)
				{
					asserv.set_arrived(true);
					printf("next spot\r\n");
					asserv.set_asserv_mode(asserv_no);
				}
				printf("asserv_vector %d");
//				control.robot_go_distance(dist + left_distance,ratio);
				control.run_wheel(left_t);
				control.run_wheel(right_t);
			case(asserv_xy):

				control.distance_arc(position.get_coordinates(),control.get_destination(),&dist,&ratio);
				printf("distanceXY %f %f  ",dist,ratio);
				if(dist*dist <0.1 
				// &&	pow(position.speed(left_t),2)<0.1 
				// &&	pow(position.speed(right_t),2)<0.1 
				)
				{
					printf("exit  asserv_xy, dist = %f\r\n",dist);
					asserv.set_asserv_mode(asserv_theta);
//					asserv.set_asserv_mode(asserv_no);
				}
				control.robot_go_distance(dist,ratio);
			

				printf("asserv_vector %d");
				control.run_wheel(left_t);
				control.run_wheel(right_t);
			break;
			case(asserv_theta):
				control.go_angle();
				printf("asserv_theta %d");
				control.run_wheel(left_t);
				control.run_wheel(right_t);
				if(pow(control.get_destination().theta - position.theta(),2) <0.000001)
					{
						asserv.set_arrived(true);
					}
			break;
			case(asserv_no):
				asserv.set_arrived(true);
				control.set_speed(left_t, 0);
				control.set_speed(right_t, 0);
				printf("asserv_no %d");
				control.run_wheel(left_t);
				control.run_wheel(right_t);
			break;
		}
				rc_usleep(robot_parameter.period());
	}
	pthread_exit(NULL);
		
}
void Asserv::go_destination(float x, float y, float theta){
	coordinates_t destination;
	destination.x = x;
	destination.y = y;
	destination.theta = theta;
	control.set_destination(destination);
	asserv_mode = asserv_xy;
	arrived = false;
	while(arrived == false && rc_get_state() != EXITING)
		rc_usleep(robot_parameter.period());
}
void Asserv::go_angle(float theta){
	coordinates_t destination;
	destination.theta = theta;
	control.set_destination(destination);
	asserv_mode = asserv_theta;
	arrived = false;
	while(arrived == false && rc_get_state() != EXITING)
		rc_usleep(robot_parameter.period());
}
// void Asserv::go_destination(vector<move_t> move_table){
	// control.set_destination(move_table[0].position);
	// asserv_mode = asserv_vector;
	// arrived = false;
	// while(move_table.size()!=0 && rc_get_state() != EXITING)
	// {
		// rc_usleep(robot_parameter.period());
		// if(arrived==true)
	// }
// }
void Asserv::add_move(vector<move_t> move_table,float x, float y, float theta){
	move_t move;
	move.position.x = x;
	move.position.y = y;
	move.position.theta = theta;
	float dist,ratio;
	if(move_table.size()>0)
	{
		control.distance_arc(move_table[move_table.size()-1].position,move.position,&dist,&ratio);
		dist+=move_table[move_table.size()-1].distance;
	}
	else
		control.distance_arc(position.get_coordinates(),move.position,&dist,&ratio);
	move.distance= dist;
	move_table.push_back(move);
}
void Asserv::add_coordinate(coordinates_t* end,coordinates_t origin, coordinates_t next)
{
	printf("add_ccordinates %f %f %f, %f %f %f, %f %f %f\r\n", end->x,end->y,end->theta, origin.x,origin.y,origin.theta,next.x,next.y,next.theta);
    end->x= origin.x - next.x*sin(origin.theta)+next.y*cos(origin.theta);
    end->y= origin.y + next.x*cos(origin.theta)+next.y*sin(origin.theta);
    end->theta= origin.theta + next.theta;
	printf("add_ccordinates %f %f %f, %f %f %f, %f %f %f\r\n", end->x,end->y,end->theta, origin.x,origin.y,origin.theta,next.x,next.y,next.theta);

}
Asserv::Asserv(){
	asserv_mode = asserv_no;
}

void Asserv::asserv_init(){
	rc_enable_motors();
	robot_parameter.load_values();

	
	if (pthread_create(&position_thread, NULL, position_computation, NULL)) {
		perror("pthread_create");
    }
	if (pthread_create(&control_thread, NULL, control_wheels, NULL)) {
		perror("pthread_create");
    }
	position.update_x(0);
	position.update_y(0);
	position.update_theta(PI/2);
}
Asserv::~Asserv(){
	pthread_join(position_thread, NULL);
	pthread_join(control_thread, NULL);
}

