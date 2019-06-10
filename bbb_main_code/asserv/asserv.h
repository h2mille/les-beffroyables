#ifndef ASSERV_H
#define ASSERV_H

#include <queue>
#include <stack>
#include "control.h"
#include <mutex>

using namespace std;

extern Position position;
extern Control control;


typedef struct{
	coordinates_t position;
	float distance;
}move_t;

typedef enum{
	asserv_queue =3,
	asserv_xy = 2,
	asserv_theta = 1,
	asserv_no = 0
}asserv_type_t;

static void *position_computation(void *arg);
static void *control_wheels(void *arg);
static void *run_wheels(void *arg);


class Asserv{
    
    public:
	Asserv();
	~Asserv();
	void asserv_init();
	void go_destination(float x, float y, float theta);
	void go_angle(float theta);
	void go_destination();
	void add_coordinate(coordinates_t* end,coordinates_t origin, coordinates_t next);
	void set_asserv_mode(asserv_type_t asserv);
	void set_arrived(bool isit);
	void add_move(float x, float y, float theta);
	void get_next_move(coordinates_t* position, float* dist);
	void go_next();
	bool is_arrived();
	void clean_move();
	void set_pwm(int motor,float pwm);
	void motor_lock();
	void motor_unlock();
	bool motor_get_lock();
    void set_STOP(bool value);
    bool get_STOP();
	void set_v(float arg,int motor);
	float get_v(int motor);
    void set_lidar();
    void unset_lidar();
    bool is_lidar();
    void set_large();
    void set_thin();
    bool is_large();

	void force_direction(asserv_direction_t direction_value);
	asserv_direction_t get_direction();
	

	asserv_type_t get_asserv_mode();
    pthread_t lidar_thread;

    private:
	float left_distance;

	asserv_type_t asserv_mode;
	bool arrived;
	
	queue<coordinates_t> dest_list;
	stack<float> dist_list;
	
	asserv_direction_t direction;
	
    bool motor_mutex;
	pthread_t position_thread;
	pthread_t control_thread;
	pthread_t wheels_thread;
	pthread_t lidar_watchdog_thread;
    bool STOP;
	float wheel_speed_left_v;
	float wheel_speed_right_v;
    
    bool activate_detection;
    bool large;

};

#endif