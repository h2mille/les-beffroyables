#ifndef SERVO_H
#define SERVO_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
using namespace std;

static void *servo_update(void *arg);




class Servo{
    
    public:
		Servo(uint8_t id);
		~Servo();
        void CheckToggle();
        void set_toggle(bool value);
        void set_period(float value);
		void init(bool enable);
		void set_pos(float pos);
		float get_pos();
		uint8_t get_id();
		void enable();
		void disable();
		bool is_enable();

    private:
		uint8_t id_servo;
		float position;
		bool init_value;
		bool enable_value;
		pthread_t thread_id;
        bool toggle;
        int count;
        int period;
};

#endif