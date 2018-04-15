#include "encoder.h"
#include <iostream>
#include <math.h>
using namespace std;
#include "parameter.h"
#include "types.h"

#ifdef USE_GYRO
rc_imu_data_t data;
#endif

int prev_L =0;
int prev_R =0;
extern parameter robot_parameter;
uint64_t enc_time_from_last_call;
uint32_t enc_time_diff;
#include <inttypes.h>
void encoder::get_move(coordinates_t* coordinates_move,coordinates_t* speed_coordinates, float* speed, float* distance)
{
	uint64_t time_temp=rc_nanos_since_boot();
	enc_time_diff=(uint32_t)(time_temp-enc_time_from_last_call);
//	printf("encoder real time:%d %" PRId64 " %" PRId64 "\n", enc_time_diff,time_temp, enc_time_from_last_call);
	enc_time_from_last_call=time_temp;
	
	int L=rc_get_encoder_pos(left_t+1)-prev_L;
	int R=rc_get_encoder_pos(right_t+1)-prev_R;
	prev_L = rc_get_encoder_pos(left_t+1);
	prev_R = rc_get_encoder_pos(right_t+1);
	float L_mm = (robot_parameter.left_wheel_diameter()  *L * PI )/ robot_parameter.dotperturn();
	float R_mm = (robot_parameter.right_wheel_diameter() *R * PI )/ robot_parameter.dotperturn();
#ifdef USE_GYRO

	
	rc_imu_data_t data; //struct to hold new data
	rc_read_gyro_data(&data);
	printf("GYRO %6.6f",data.gyro[2]);
	coordinates_move->theta = (-data.gyro[2]*90)-0.0008;
	coordinates_move->theta *=((float)enc_time_diff);
#else

	coordinates_move->theta = (R_mm - L_mm)/robot_parameter.wheel_distance();
#endif
	coordinates_move->x = ((L_mm + R_mm)/2) * sin(coordinates_move->theta);
	coordinates_move->y = ((L_mm + R_mm)/2) * cos(coordinates_move->theta);
	speed_coordinates->x     = (coordinates_move->x     * 1000000000/(float)enc_time_diff);
	speed_coordinates->y     = (coordinates_move->y     * 1000000000/(float)enc_time_diff);
	speed_coordinates->theta = (coordinates_move->theta * 1000000000/(float)enc_time_diff);
	

	speed[left_t]=L_mm * 1000000000/(float)enc_time_diff;
	speed[right_t]=R_mm * 1000000000/(float)enc_time_diff;
	distance[left_t]=L_mm ;
	distance[right_t]=R_mm ;
	return;
	
}




