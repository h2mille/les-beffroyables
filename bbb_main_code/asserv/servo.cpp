#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "encoder.h"
#include "position.h"
#include "control.h"
#include "parameter.h"
#include "rc_usefulincludes.h"
#include <pthread.h>
#include "servo.h"

using namespace std;

#include "roboticscape.h"
#include "types.h"

Servo servo0(1);
Servo servo1(2);
Servo servo2(3);
Servo servo3(4);
Servo servo4(5);
Servo servo5(6);
Servo servo6(7);
Servo servo7(8);

static void *servo_computation(void *arg){
	Servo * self = (Servo *) arg;
	while(rc_get_state() != EXITING){
		if(self->is_enable())
		{
			rc_send_servo_pulse_normalized(self->get_id(),self->get_pos());
		}
		rc_usleep(1000000/50);
	}
	pthread_exit(NULL);
}

Servo::Servo(uint8_t id):id_servo(id),init_value(false)
{
}

Servo::~Servo(){
	pthread_join(thread_id, NULL);
}

void Servo::init(bool enable_=true)
{
	enable_value=enable_;
	if (pthread_create(&thread_id, NULL, servo_computation, this)) {
		perror("pthread_create");
    }	
	else
		init_value=true;
}

void Servo::set_pos(float pos){
	if(init_value==false)
	{
		init();
	}
	position=pos;
}
float Servo::get_pos(){
	return position;
}
uint8_t Servo::get_id(){
	return id_servo;
}

bool Servo::is_enable()
{
	return enable_value;
}

void Servo::enable()
{
	enable_value=true;
}
void Servo::disable()
{
	enable_value=false;
}