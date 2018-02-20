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
#define couleur(param) printf("\033[%sm",param)



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
//		 printf("\r");
//		 position.print_coordinates();
		fflush(stdout);
		rc_usleep(robot_parameter.period());

	}
	pthread_exit(NULL);
		
}

static void *control_wheels(void *arg){
FILE *f ;
	while(rc_get_state() != EXITING){
		float dist;
		float ratio;
		//couleur("34");
		switch(asserv.get_asserv_mode())
		{
			case(asserv_queue):
			    coordinates_t pos;
				float dist_left;
				
				
			    asserv.get_next_move(&pos,&dist_left);
				control.distance_arc(position.get_coordinates(),pos,&dist,&ratio,force_frontward);
				if(dist<0)
					dist=-dist;
				printf("\r\ndistance %f %f\r\n",dist+dist_left,ratio);
				printf("%f;%f;%f;%f;%f;%f\n", position.get_coordinates().x,position.get_coordinates().y,position.get_coordinates().theta,(dist+dist_left),dist, ratio);
				printf("from %f %f to %f %f\n", position.get_coordinates().x,position.get_coordinates().y,pos.x,pos.y);

				f=fopen("log.csv", "a");
				fprintf(f, "%f;%f;%f;%f;%f;%f\n", position.get_coordinates().x,position.get_coordinates().y,position.get_coordinates().theta,(dist+dist_left),dist, ratio);
				fclose(f);

				if((dist)*(dist) <125	)
				{
					if((dist+dist_left)*(dist+dist_left)<2)
					{
						asserv.set_arrived(true);
						printf("end trip\r\n");
						asserv.set_asserv_mode(asserv_no);
					}
					else
					{
						couleur("31");
						printf("\r\nnext spot\r\n");
						asserv.go_next();
					}
				}
				printf("\r\nasserv_vector\r\n");
				couleur("0");
				control.robot_go_distance(dist + dist_left,ratio);
				control.run_wheel(left_t);
				control.run_wheel(right_t);
				break;
			case(asserv_xy):

				control.distance_arc(position.get_coordinates(),control.get_destination(),&dist,&ratio,asserv_any_dir);
				printf("distanceXY %f %f  ",dist,ratio);
				if(dist*dist <0.4)
				{
					printf("exit  asserv_xy, dist = %f\r\n",dist);
					asserv.set_asserv_mode(asserv_theta);
				}
				couleur("0");
				control.robot_go_distance(dist,ratio);
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
				couleur("0");
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
 void Asserv::go_destination(){
	 control.set_destination(dest_list.front());
	 asserv_mode = asserv_queue;
	 arrived = false;
	 while(arrived==false && rc_get_state() != EXITING)
	 {
		 rc_usleep(robot_parameter.period());

	 }
 }
void Asserv::add_move(float x, float y, float theta){
	coordinates_t move;
	move.x = x;
	move.y = y;
	move.theta = theta;
	float dist,ratio;
	if(dest_list.size()>0)
	{
		control.distance_arc(dest_list.front(),move,&dist,&ratio,force_frontward);
		dist+=dist_list.top();
	}
	else
	{
		control.distance_arc(position.get_coordinates(),move,&dist,&ratio,force_frontward);
		dist=0;
	}
					if(dist<0)
					dist=-dist;

	printf("dist= %f\r\n", dist);
	printf("dist= %f\r\n", dist);
	dist_list.push(dist);
	dest_list.push(move);
}
void Asserv::get_next_move(coordinates_t* pos, float* dist){
	if (dest_list.empty())
	{
		pos->x = position.get_coordinates().x;
		pos->y = position.get_coordinates().y;
		pos->theta = position.get_coordinates().theta;
		*dist=0;
	}
	else
	{
		pos->x = dest_list.front().x;
		pos->y = dest_list.front().y;
		pos->theta = dest_list.front().theta;
		*dist=dist_list.top();
	}
    return;
}
void Asserv::go_next(){
	if (!dest_list.empty())
		dest_list.pop();
	if (!dist_list.empty())
		dist_list.pop();
	return;
}
void Asserv::clean_move(){
	while(!dest_list.empty())
		dest_list.pop();
	while(!dist_list.empty())
		dist_list.pop();

}

void Asserv::add_coordinate(coordinates_t* end,coordinates_t origin, coordinates_t next)
{
    end->x= origin.x - next.x*sin(origin.theta)+next.y*cos(origin.theta);
    end->y= origin.y + next.x*cos(origin.theta)+next.y*sin(origin.theta);
    end->theta= origin.theta + next.theta;

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

