#ifndef ASSERV_H
#define ASSERV_H

#include <queue>
#include <stack>
using namespace std;

extern Position position;
extern Control control;
extern parameter robot_parameter;

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
	void clean_move();

	asserv_type_t get_asserv_mode();
	bool is_arrived();
    
    private:
	float left_distance;

	asserv_type_t asserv_mode;
	bool arrived;
	
	queue<coordinates_t> dest_list;
	stack<float> dist_list;

	
	pthread_t position_thread;
	pthread_t control_thread;

};

#endif