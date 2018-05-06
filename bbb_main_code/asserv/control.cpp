#include "control.h"
#include "parameter.h"
#include "encoder.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
using namespace std;
#include <cmath>

extern parameter robot_parameter;
extern Position position;
extern uint32_t enc_time_diff;
Control control;

Control::Control()
{
	destination.x     = 0;
	destination.y     = 0;
	destination.theta = 0;
	pwm_v[left_t]=0;
	pwm_v[right_t]=0;

	pid_dist[0]=0.5;
	pid_dist[1]=0.0;
	pid_dist[2]=0.0;
	
	pid_speed[0]=0.0001;
	pid_speed[1]=0.00015;
	pid_speed[2]=-0.005;
	speed_max = 700;
	accel_max = 1000;
	rot_speed_max = 3;
	rot_accel_max = 8;
}
void Control::set_speed_accel(float s,float a)
{
	speed_max = s;
	accel_max = a;
}
void Control::set_rot_speed_accel(float s,float a)
{
	rot_speed_max = s;
	rot_accel_max = a;
}
void Control::set_pid_dist(float arg1,float arg2,float arg3)
{
	pid_dist[0]=arg1;
	pid_dist[1]=arg2;
	pid_dist[2]=arg3;
}
void Control::set_pid_speed(float arg1,float arg2,float arg3)
{
	pid_speed[0]=arg1;
	pid_speed[1]=arg2;
	pid_speed[2]=arg3;	
}


void Control::run()
{
	if(state==false)
		return;
	float delta_angle_start  = atan((position.y() - destination.y)/(position.x() - destination.x)) - position.theta() ;
	float delta_angle_finish = destination.theta - position.theta();
	float delta_distance = sqrt(pow(position.x() - destination.x,2) + pow(position.y() - destination.y,2)
	);
	if(fabs(delta_angle_start)>robot_parameter.pastille_angle_depart()){
		;
	}
	else if(fabs(delta_distance)>robot_parameter.pastille_distance()){
		;
	}
	else if(fabs(delta_angle_finish)>robot_parameter.pastille_angle_fin()){
		;
	}
	return;
}

void Control::go_angle(){
	// float pid_p = 0.5;
	// float pid_i = 0.000;
	// float pid_d = 0;

	// float rotation_speed_limit =3;
	// float rotation_acceleration_limit =8;
	float delta_angle = (destination.theta-position.theta());
	while(delta_angle>PI)
		delta_angle-=2*PI;
	while(delta_angle<-PI)
		delta_angle+=2*PI;
	float real_rotation_speed = (position.speed_coordinates().theta+prev_rotation_speed)/2;

	float rotation_speed= rot_speed_max;
	if(delta_angle<0)
		rotation_speed=-rotation_speed;

	if(rotation_speed>real_rotation_speed+rot_accel_max*(float)enc_time_diff/1000000000)
		rotation_speed=real_rotation_speed+rot_accel_max*(float)enc_time_diff/1000000000;
	else if(rotation_speed<real_rotation_speed-rot_accel_max*(float)enc_time_diff/1000000000)
		 rotation_speed=real_rotation_speed-rot_accel_max*(float)enc_time_diff/1000000000;

	if(delta_angle>0)
	{
		if(rotation_speed>sqrt(2* delta_angle/rot_accel_max) *rot_accel_max)
			rotation_speed=sqrt(2* delta_angle/rot_accel_max) *rot_accel_max;
		else if(rotation_speed<-sqrt(2* delta_angle/rot_accel_max) *rot_accel_max)
			rotation_speed=-sqrt(2* delta_angle/rot_accel_max) *rot_accel_max;
	}
	else
	{
		if(rotation_speed>sqrt(2* -delta_angle/rot_accel_max) *rot_accel_max)
			rotation_speed=sqrt(2* -delta_angle/rot_accel_max) *rot_accel_max;
		else if(rotation_speed<-sqrt(2* -delta_angle/rot_accel_max) *rot_accel_max)
			rotation_speed=-sqrt(2* -delta_angle/rot_accel_max) *rot_accel_max;
	}
	
	float wheel_speed= rotation_speed*robot_parameter.wheel_distance()/2;
	if(wheel_speed>speed_max)
		wheel_speed=speed_max;
	if(-speed_max>speed_max)
		wheel_speed=-speed_max;

	
	set_speed(left_t , -wheel_speed);
	set_speed(right_t , wheel_speed);
	prev_rotation_speed = rotation_speed;
	prev_delta_angle = delta_angle;
	
}

void Control::wheel_go_distance(wheel_t wheel){
	 float pid_p = 100000;
	 float pid_i = 0.000;
	 float pid_d = 0.0;
	float speed_limit = 1000;
	float acceleration_limit = 5000;
	
	float delta_distance = wheel_dist_destination[wheel] - position.distance(wheel);
	float wheel_speed = position.speed(wheel);
	float rotation_speed= pid_p * delta_distance+ pid_d * wheel_speed ;
	if((rotation_speed - wheel_speed)*1000000/(float)robot_parameter.period()>acceleration_limit)
		rotation_speed=acceleration_limit*(float)robot_parameter.period()/1000000+wheel_speed;
	else if((rotation_speed - wheel_speed)*1000000/(float)robot_parameter.period()<-acceleration_limit)
		rotation_speed=-acceleration_limit*(float)robot_parameter.period()/1000000+wheel_speed;
	if(rotation_speed>speed_limit)
		rotation_speed=speed_limit;
	else if(rotation_speed<-speed_limit)
		rotation_speed=-speed_limit;
	set_speed(wheel , rotation_speed);	
}

void Control::robot_go_distance(float distance, float ratio){
//	float pid_p = 100000;
	// float pid_p = 30000;
	// float pid_i = 0.000;
	// float pid_d = -3000;

	float acceleration_limit = 1000;
	
	
	// float delta_distance = distance;
	// float wheel_speed = (position.speed(left_t)+position.speed(right_t))/2;
	// float rotation_speed= pid_p * delta_distance+ pid_d * wheel_speed ;
	float delta_distance = distance;
//	float wheel_speed = (position.speed(left_t)+position.speed(right_t))/2;
	float wheel_speed = ((speed[left_t]+speed[right_t])+(position.speed(left_t)+position.speed(right_t)))/4;
	float rotation_speed= speed_max ;
	if(delta_distance<0)
		rotation_speed= -rotation_speed;
	//Check if speed is too high
	//for acceleration
//	printf("go_dist %f %f %f\r\n",delta_distance,rotation_speed,ratio);
	uint32_t temp_enc_time=enc_time_diff/10000;
	if((rotation_speed - wheel_speed)*1000000/(float)robot_parameter.period()>accel_max)
	{
		rotation_speed=accel_max*(float)robot_parameter.period()/1000000+wheel_speed;
	}
	else if((rotation_speed - wheel_speed)*1000000/(float)robot_parameter.period()<-accel_max)
	{
		rotation_speed=-accel_max*(float)robot_parameter.period()/1000000+wheel_speed;
	}
	//for deceleration
	// if(rotation_speed>delta_distance*accel_max)
	if(delta_distance>0)
	{
		if(rotation_speed>sqrt(2* delta_distance/accel_max) *accel_max)
		{
			rotation_speed=sqrt(2* delta_distance/accel_max)*accel_max;
		}
		if(-rotation_speed>sqrt(2* delta_distance/accel_max) *accel_max)
		{
			rotation_speed=-sqrt(2* delta_distance/accel_max)*accel_max;
		}
	}
	else
	{
		if(rotation_speed>sqrt(2* -delta_distance/accel_max) *accel_max)
		{
			rotation_speed=sqrt(2* -delta_distance/accel_max)*accel_max;
		}
		if(-rotation_speed>sqrt(2* -delta_distance/accel_max) *accel_max)
		{
			rotation_speed=-sqrt(2* -delta_distance/accel_max)*accel_max;
		}
	} 
	printf("go_dist %f %f %f\r\n",delta_distance,rotation_speed, ratio);
	
	
	//set speed
	float left_speed;
	float right_speed;
	if(ratio!=-1)
	{
		if(distance>0)
		{
			left_speed =  rotation_speed * (2/(ratio+1));
			right_speed = rotation_speed * (2/(ratio+1))*ratio;
		}
		else
		{
			left_speed =  rotation_speed * (2/(ratio+1))*ratio;
			right_speed = rotation_speed * (2/(ratio+1));
		}
	}else{
		if(distance>0)
		{
			left_speed =  -rotation_speed ;
			right_speed = -rotation_speed ;
		}
		else
		{
			left_speed =  rotation_speed ;
			right_speed = rotation_speed ;
		}
	}
	//check both wheels can be fast enough, else reduce both speeds
	if(left_speed>speed_max || left_speed<-speed_max)
	{
		right_speed = right_speed *speed_max/left_speed;
		left_speed = speed_max;
	}
	if(right_speed>speed_max || right_speed<-speed_max)
	{
		left_speed = left_speed *speed_max/right_speed;
		right_speed = speed_max;
	}
	printf("will wheel speed  %f %f %\r\n",left_speed,right_speed);
	set_speed(left_t , left_speed );	
	set_speed(right_t , right_speed );
}





void Control::set_speed(wheel_t wheel, float set_speed){
	speed[wheel]=set_speed;
}
void Control::print_speed(){
	printf("speed %f %f ", speed[0],speed[1]);
	printf("angles %f %f \r\n", destination.theta,position.theta());
}

void Control::run_wheel(wheel_t wheel){
	// float pid_p = 0.01;
	// float pid_i = 0.000;
	// float pid_d = -0.05;
	float delta_speed = speed[wheel] - position.speed(wheel);
	pwm_v[wheel]+= pid_speed[0] * delta_speed + pid_speed[1] *(prev_delta_speed[wheel] + delta_speed) + pid_speed[2] *(prev_delta_speed[wheel] - delta_speed) ;
	if(pwm_v[wheel]<-1)
		pwm_v[wheel]=-1;
	else if (pwm_v[wheel]>1)
		pwm_v[wheel]=1;
	switch(wheel){
		case left_t:
			rc_set_motor(1, pwm_v[wheel]);
			break;
		case right_t:
			rc_set_motor(2, pwm_v[wheel]);
			break;
		default:
			break;
	}
	prev_delta_speed[wheel] = delta_speed;
	return;	
}



void Control::set_destination(coordinates_t destination_coordinates)
{
	destination.x     = destination_coordinates.x;
	destination.y     = destination_coordinates.y;
	destination.theta = destination_coordinates.theta;
}
coordinates_t Control::get_destination()
{
	return destination;
}

void Control::set_wheel_distance(wheel_t wheel, float distance){
	position.Clear_Distance_wheel();
	wheel_dist_destination[wheel] = distance;
	
}

void Control::enable()
{
	state=true;
}
void Control::disable()
{
	state=false;
}

void Control::distance_direct_arc( coordinates_t stop, float* distance, float* ratio,asserv_direction_t dir)
{
	
//	printf("equivalent to going to %f %f %f \r\n",stop.x,stop.y,stop.theta);
	float theta_in,theta_side;
	if(stop.y!=0)
		theta_in = PI/2-atan(stop.x/stop.y);
	else
		theta_in = 0;
	
	theta_side = PI - 2*theta_in;

	if(theta_side == 0)
	{
		*distance = stop.y;
		*ratio = 1;
		return;
	}
	float d = sqrt(pow(stop.x,2)+pow(stop.y,2));

	float r=d/(2*cos(theta_in));
	
	
	switch(dir)
	{
	case force_forward:
		if(stop.y>=0)
		{
//			printf("1 %f %f %f \r\n",d,theta_side,r);
			*distance  = theta_side*r;
			*ratio = (r-robot_parameter.wheel_distance()/2)/(r+robot_parameter.wheel_distance()/2);
//			printf("12 %f %f \r\n",*distance,*ratio);
		}
		else{
//			printf("21 %f %f \r\n",theta_side,r);
			*distance  = (theta_side)*r;
			*ratio = (r+robot_parameter.wheel_distance()/2)/(r-robot_parameter.wheel_distance()/2);
//			printf("22 %f %f \r\n",*distance,*ratio);
		}
	break;
	case force_backwardward:
		if(stop.y>=0)
		{
//			printf("3 %f %f \r\n",theta_side,r);
			*distance  = -(2*PI-theta_side)*r;
			*ratio = (r+robot_parameter.wheel_distance()/2)/(r-robot_parameter.wheel_distance()/2);
		}
		else{
//			printf("4 %f %f \r\n",theta_side,r);
			*distance  = -theta_side*r;
			*ratio = (r-robot_parameter.wheel_distance()/2)/(r+robot_parameter.wheel_distance()/2);
		}
	break;
	case asserv_any_dir:
		if(stop.y>=0)
		{
//			printf("5\r\n");
			*distance  = theta_side*r;
			*ratio = (r-robot_parameter.wheel_distance()/2)/(r+robot_parameter.wheel_distance()/2);
		}
		else{
//			printf("6\r\n");
			*distance  = -theta_side*r;
			*ratio = (r-robot_parameter.wheel_distance()/2)/(r+robot_parameter.wheel_distance()/2);
		}
	break;
	}

	return;
}

void Control::distance_arc( coordinates_t start, coordinates_t stop, float* distance, float* ratio,asserv_direction_t dir){
	coordinates_t temp,temp2;
	float angle = start.theta-PI/2;
//	printf("check coordinates start: %f %f %f ,stop: %f %f %f", start.x,start.y,start.theta,stop.x,stop.y,stop.theta);
	while(angle>PI)
		angle-=2*PI;
	while(angle<-PI)
		angle+=2*PI;
	temp.x = stop.x - start.x;
	temp.y = stop.y - start.y;
	float cosa, sina;
	cosa = cos(angle);
	sina = sin(angle);

	temp2.x =   temp.x * cosa + temp.y *sina;
	temp2.y =  -temp.x * sina + temp.y *cosa;

	distance_direct_arc(temp2, distance, ratio,dir);
	return;
}
