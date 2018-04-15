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

extern Position position;
extern Control control;
extern parameter robot_parameter;

mutex motor_mutex;

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
		printf("asserv is unlocked\r\n");
		switch(asserv.get_asserv_mode())
		{
			case(asserv_queue):
			    coordinates_t pos;
				float dist_left;
				
				


			    asserv.get_next_move(&pos,&dist_left);
				control.distance_arc(position.get_coordinates(),pos,&dist,&ratio,asserv.get_direction());

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
						asserv.get_next_move(&pos,&dist_left);
						control.distance_arc(position.get_coordinates(),pos,&dist,&ratio,asserv.get_direction());
					}
				}



				if(dist<0)
					dist=-dist;
				printf("\r\ndistance %f %f\r\n",dist+dist_left,ratio);
				printf("%f;%f;%f;%f;%f;%f\n", position.get_coordinates().x,position.get_coordinates().y,position.get_coordinates().theta,(dist+dist_left),dist, ratio);
				printf("from %f %f to %f %f\n", position.get_coordinates().x,position.get_coordinates().y,pos.x,pos.y);

				f=fopen("log.csv", "a");
				fprintf(f, "%f;%f;%f;%f;%f;%f\n", position.get_coordinates().x,position.get_coordinates().y,position.get_coordinates().theta,(dist+dist_left),dist, ratio);
				fclose(f);

				printf("\r\nasserv_vector\r\n");
				couleur("0");
				control.robot_go_distance(dist + dist_left,ratio);
				break;
			case(asserv_xy):

				control.distance_arc(position.get_coordinates(),control.get_destination(),&dist,&ratio,asserv.get_direction());
				printf("distanceXY %f %f  ",dist,ratio);
				if(dist*dist <0.4)
				{
					printf("exit  asserv_xy, dist = %f\r\n",dist);
					asserv.set_asserv_mode(asserv_theta);
				}
				couleur("0");
				control.robot_go_distance(dist,ratio);
				break;
			case(asserv_theta):
				control.go_angle();
				printf("asserv_theta %d");
				if(pow(control.get_destination().theta - position.theta(),2) <0.000001)
					{
						asserv.set_arrived(true);
					}
				break;
			case(asserv_no):
				asserv.set_arrived(true);
				// control.set_speed(left_t, 0);
				// control.set_speed(right_t, 0);

				couleur("0");
				break;
		}
		rc_usleep(robot_parameter.period());
	}
	pthread_exit(NULL);
		
}

static void *run_wheels(void *arg)
{
	while(rc_get_state() != EXITING)
	{
		motor_mutex.lock();
		control.run_wheel(left_t);
		control.run_wheel(right_t);
		motor_mutex.unlock();
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
}
void Asserv::go_angle(float theta){
	coordinates_t destination;
	destination.theta = theta;
	control.set_destination(destination);
	asserv_mode = asserv_theta;
	arrived = false;
}
 void Asserv::go_destination(){
	 control.set_destination(dest_list.front());
	 asserv_mode = asserv_queue;
	 arrived = false;
 }
void Asserv::add_move(float x, float y, float theta){
	coordinates_t move;
	move.x = x;
	move.y = y;
	move.theta = theta;
	float dist,ratio;
	if(dest_list.size()>0)
	{
		control.distance_arc(dest_list.back(),move,&dist,&ratio,direction);
		dist+=dist_list.top();
	}
	else
	{
		control.distance_arc(position.get_coordinates(),move,&dist,&ratio,direction);
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
	coordinates_t temp;
	temp.x=origin.x;
	temp.y=origin.y;
	temp.theta=origin.theta;
    end->x= temp.x - next.x*sin(temp.theta)+next.y*cos(temp.theta);
    end->y= temp.y + next.x*cos(temp.theta)+next.y*sin(temp.theta);
    end->theta= temp.theta + next.theta;

}

Asserv::Asserv(){
	asserv_mode = asserv_no;
	direction=asserv_any_dir;
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
	if (pthread_create(&wheels_thread, NULL, run_wheels, NULL)) {
		 perror("pthread_create");
     }
	position.update_x(0);
	position.update_y(0);
	position.update_theta(PI/2);
}
Asserv::~Asserv(){
	pthread_join(position_thread, NULL);
	pthread_join(control_thread, NULL);
	pthread_join(wheels_thread, NULL);
}
void Asserv::set_pwm(int motor, float pwm){
	rc_set_motor(motor, pwm);
}
void Asserv::motor_lock(){
	printf("lock asserv\r\n");
	motor_mutex.lock();
}
void Asserv::motor_unlock(){
	printf("unlock asserv\r\n");
	motor_mutex.unlock();
}

void Asserv::force_direction(asserv_direction_t direction_value)
{
	direction=direction_value;
}

asserv_direction_t Asserv::get_direction()
{
	return direction;
}
