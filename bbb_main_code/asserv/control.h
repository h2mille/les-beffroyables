#ifndef CONTROL_H
#define CONTROL_H
#include <stdint.h>
#include "position.h"
#include "../../Robotics_Cape_Installer/libraries/rc_usefulincludes.h"
#include "../../Robotics_Cape_Installer/libraries/roboticscape.h"
#include "types.h"




typedef enum{
	force_forward,
	force_backwardward,
	asserv_any_dir
}asserv_direction_t;

class Control{
    
    public:
	Control();
	void set_pid_dist(float arg1,float arg2,float arg3);
	void set_pid_speed(float arg1,float arg2,float arg3);
    void run();
	void go_angle();
	void wheel_go_distance(wheel_t wheel);
	void robot_go_distance(float distance, float ratio);
	void set_speed(wheel_t wheel, float set_speed);
	void print_speed();
	void run_wheel(wheel_t wheel);
    void set_destination(coordinates_t destination_coordinates);
    coordinates_t get_destination();
    void set_wheel_distance(wheel_t wheel, float distance);
	
	void set_speed_accel(float s,float a);
	void set_rot_speed_accel(float s,float a);

    void enable();
    void disable();

    void distance_direct_arc( coordinates_t stop, float* distance, float* ratio,asserv_direction_t dir);
	void distance_arc( coordinates_t start, coordinates_t stop, float* distance, float* ratio,asserv_direction_t dir);

    
    private:

    coordinates_t destination; 
	bool state;
	float pid_dist[3];
	float pid_speed[3];
	
	float pwm_v[2];
	float speed[2];
	float prev_delta_speed[2];
	float prev_delta_angle;
	float prev_rotation_speed;
	float wheel_dist_destination[2];
	
	float speed_max;
	float accel_max;
	float rot_speed_max;
	float rot_accel_max;

    
};

#endif
