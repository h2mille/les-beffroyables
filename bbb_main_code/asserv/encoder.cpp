#include "encoder.h"
#include <iostream>
#include <math.h>
using namespace std;
#include "parameter.h"
#include "types.h"



int prev_L =0;
int prev_R =0;
extern parameter robot_parameter;

void encoder::get_move(coordinates_t* coordinates_move,coordinates_t* speed_coordinates, float* speed, float* distance)
{
	
	int L=rc_get_encoder_pos(left_t+1)-prev_L;
	int R=rc_get_encoder_pos(right_t+1)-prev_R;
	prev_L = rc_get_encoder_pos(left_t+1);
	prev_R = rc_get_encoder_pos(right_t+1);
	float L_mm = (robot_parameter.left_wheel_diameter()  *L * PI )/ robot_parameter.dotperturn();
	float R_mm = (robot_parameter.right_wheel_diameter() *R * PI )/ robot_parameter.dotperturn();

	coordinates_move->theta = (R_mm - L_mm)/robot_parameter.wheel_distance();
	coordinates_move->x = ((L_mm + R_mm)/2) * sin(coordinates_move->theta);
	coordinates_move->y = ((L_mm + R_mm)/2) * cos(coordinates_move->theta);
	speed_coordinates->x     = (coordinates_move->x     * 1000000/(float)robot_parameter.period());
	speed_coordinates->y     = (coordinates_move->y     * 1000000/(float)robot_parameter.period());
	speed_coordinates->theta = (coordinates_move->theta * 1000000/(float)robot_parameter.period());
	

	speed[left_t]=L_mm * 1000000/(float)robot_parameter.period();
	speed[right_t]=R_mm * 1000000/(float)robot_parameter.period();
	distance[left_t]=L_mm ;
	distance[right_t]=R_mm ;
	return;
	
}




