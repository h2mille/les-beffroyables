#ifndef CONTROL_H
#define CONTROL_H
#include <stdint.h>
#include "position.h"
#include "../../Robotics_Cape_Installer/libraries/rc_usefulincludes.h"
#include "../../Robotics_Cape_Installer/libraries/roboticscape.h"
#include "types.h"


extern Position position;

typedef enum{
	force_frontward,
	force_backwardward,
	asserv_any_dir
}asserv_direction_t;

class Control{
    
    public:
	Control();
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

    void enable();
    void disable();

    void distance_direct_arc( coordinates_t stop, float* distance, float* ratio,asserv_direction_t dir);
	void distance_arc( coordinates_t start, coordinates_t stop, float* distance, float* ratio,asserv_direction_t dir);

    
    private:

    coordinates_t destination; 
	bool state;
	float pwm_v[2];
	float speed[2];
	float prev_delta_speed[2];
	float prev_delta_angle;
	float prev_rotation_speed;
	float wheel_dist_destination[2];
    
};

#endif
