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
	asserv_queue,
	asserv_xy,
	asserv_theta,
	asserv_no
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

	void force_direction(asserv_direction_t direction_value);
	asserv_direction_t get_direction();
	

	asserv_type_t get_asserv_mode();
    
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
	

};

#endif