#include "position.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include "encoder.h"


using namespace std;


Position position;

Position::Position()
{
    coordinates.x=0;
    coordinates.y=0;
    coordinates.theta=PI/2;
    speed_coordinates_v.x=0;
    speed_coordinates_v.y=0;
    speed_coordinates_v.theta=0;
	distance_v[left_t]=0;
	distance_v[left_t]=0;
}


coordinates_t Position::get_coordinates(){   
    return coordinates;
}
float Position::x(){
	return coordinates.x;
}
float Position::y(){
	return coordinates.y;
}
float Position::theta(){
	return coordinates.theta;
}
float Position::speed(wheel_t wheel){
	return speed_v[wheel];
}
float Position::distance(wheel_t wheel){
	return distance_v[wheel];
}
coordinates_t Position::speed_coordinates(){
	return speed_coordinates_v;
}

uint8_t Position::set_coordinates(coordinates_t new_coordinate){
    coordinates.x=new_coordinate.x;
    coordinates.y=new_coordinate.y;
    coordinates.theta=new_coordinate.theta;
    return true;
}
uint8_t Position::add_coordinates(coordinates_t new_coordinate){
    coordinates.x+= -new_coordinate.x*sin(coordinates.theta)+new_coordinate.y*cos(coordinates.theta);
    coordinates.y+=new_coordinate.x*cos(coordinates.theta)+new_coordinate.y*sin(coordinates.theta);
    coordinates.theta+=new_coordinate.theta;
    return true;
}
uint8_t Position::update_x(float new_coordinate){
    coordinates.x=new_coordinate;
    return true;
}
uint8_t Position::update_y(float new_coordinate){
    coordinates.y=new_coordinate;
    return true;
}
uint8_t Position::update_theta(float new_coordinate){
    coordinates.theta=new_coordinate;
    return true;
}

void Position::print_coordinates(){
	printf("Position: %f, %f, %f Speed : %f %f",coordinates.x,coordinates.y,coordinates.theta*RAD_TO_DEG,speed_v[left_t],speed_v[right_t]);
}

void Position::Update_coordinates(){
	coordinates_t move_coordinates;
	float_t temp_distance[2];
	encoder::get_move(&move_coordinates, &speed_coordinates_v,speed_v,temp_distance);
	distance_v[left_t]+=temp_distance[left_t];
	distance_v[right_t]+=temp_distance[right_t];
	add_coordinates(move_coordinates);
	real_theta = coordinates.theta;
	while(real_theta>2*PI)
		real_theta-=(2*PI);
	while(real_theta<0)
		real_theta+=(2*PI);
}
void Position::Clear_Distance_wheel(){
	distance_v[left_t] = 0;
	distance_v[right_t] = 0;
}
