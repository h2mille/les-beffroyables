#ifndef POSITION_H
#define POSITION_H
#include <stdint.h>

#include "types.h"



class Position{
    
    public:
    Position();
    coordinates_t get_coordinates();
	float x();
	float y();
	float theta();
	float distance(wheel_t);
	
	float speed(wheel_t wheel);
	coordinates_t speed_coordinates();
    uint8_t set_coordinates(coordinates_t new_coordinate);
    uint8_t add_coordinates(coordinates_t new_coordinate);
    uint8_t update_x(float new_coordinate);
    uint8_t update_y(float new_coordinate);
    uint8_t update_theta(float new_coordinate);
    void print_coordinates();
	void Update_coordinates();
	void Clear_Distance_wheel();
    
    private:
    coordinates_t coordinates;
    float real_theta;
    coordinates_t speed_coordinates_v;
    float speed_v[2];
    float distance_v[2];
    
    
};

#endif