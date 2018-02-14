#ifndef ASSERV_H
#define ASSERV_H

#include <vector>
using namespace std;

extern Position position;
extern Control control;
extern parameter robot_parameter;

typedef struct{
	coordinates_t position;
	float distance;
}move_t;

typedef enum{
	asserv_vector,
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
	// void go_destination(vector<move_t> move_table);
	void add_coordinate(coordinates_t* end,coordinates_t origin, coordinates_t next);
	void set_asserv_mode(asserv_type_t asserv);
	void set_arrived(bool isit);
	void add_move(vector<move_t> move_table,float x, float y, float theta);

	asserv_type_t get_asserv_mode();
	bool is_arrived();
    
    private:
	float left_distance;

	asserv_type_t asserv_mode;
	bool arrived;

	
	pthread_t position_thread;
	pthread_t control_thread;

};

#endif