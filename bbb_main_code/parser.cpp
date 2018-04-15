
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "parser.h"
#include <pthread.h>
#include "asserv/servo.h"
#include "asserv/position.h"
#include "asserv/asserv.h"
#include "asserv/control.h"

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;


#include <iostream>
#include <stdio.h>
#include <unistd.h>


#include "../Robotics_Cape_Installer/libraries/rc_usefulincludes.h"
#include "../Robotics_Cape_Installer/libraries/roboticscape.h"


extern Servo servo0;
extern Servo servo1;
extern Servo servo2;
extern Servo servo3;
extern Servo servo4;
extern Servo servo5;
extern Servo servo6;
extern Servo servo7;

extern Asserv asserv;
extern Position position;
extern Control control;
/*
langage:
-x valeur en x
-y valeur en y
-t valeur en theta
-m mode de déplacement (avant:1, arrière:-1; indifferent :0)

-r relatif


-G aller à un point donné (x,y,t,m,a/r)
-T tourner à un angle donné (t,a/r)


-S Tourner le servo (t, de -1.5 à +1.5)
-P afficher la postion 
-E etat actuel de l'asservissement

-A ajouter position (x,y)
-L lancer une trajectoire (-m)
-E efface la trajectoire

- stopper
- reprendre

-D regler pid (-m=0 speed, -m=1 dist, x=P, y=I,t=D)
-X regler vitesse angulaire, acceleration angulaire (-m=0 lineaire, -m=1 angluaire dist, x=vitesse, y=Acceleration)
-Y regler position
-V obtenir vitesse
-B pwm d'un moteur (-x PWM, -m0-1=desactiver asserv -m0=reactiver asserv -mn moteur n)

*/

enum action_t
{
	go_,
	turn_,
	servo_,
	position_,
	state_,
	add_,
	launch_,
	erase_,
	pid_,
	speed_accel_set_,
	set_pos_,
	get_speed_,
	motor_pwm_,
	none_
};

Parser parser;

static void *parser_function(void *arg){ 
	umask(0000);
    mkfifo(myfifo_in, 0666);
    mkfifo(myfifo_out, 0666);
	 int fd1;
	// fd_set fds;
	// timeval tv ={0,100};//(sec,usec)
	int size,i;


	char str1[80];
	char *argv[80];
	int argc=0;
	
	float   x_arg;
	float   y_arg;
	float   theta_arg;
	int     mode_arg;
	
	

	
	
	argv[0]=&str1[0];
	argc++;
	while(rc_get_state() != EXITING){
		fd1 = open(myfifo_in,O_RDONLY|O_NONBLOCK);		
		size = read(fd1, str1, 80);
		close(fd1);
		if(size>0)
		{
//on nettoie la chaine
			printf("receive : %s",myfifo_in);

			argc=0;
			for(i=0;(i<size);i++)
			{
				if((str1[i]=='-')||(i==0))
				{
					argv[argc]=&str1[i];
					argc++;
				}
				if(str1[i]=='\r'||str1[i]=='\n')
					str1[i]='\0';
			}
//on parse la chaine
			int c, in;
			float in_f;
			extern char *optarg;
			optind=1;
			float   x_arg;
			float   y_arg;
			float   theta_arg;
			int     mode_arg;
			uint8_t var_init=0;
			bool action_done=false;
			action_t action=none_;
			bool absolute= true;

		 	while ((c = getopt(argc, argv, "x:y:t:m:rGTDSPEALEXYVB")) != -1){
				switch (c){
				case 'x': // motor channel option
					x_arg = atof(optarg);
					printf("x: %f\n", x_arg);

					var_init|=0b0000001;
					break;
				case 'y': // motor channel option
					y_arg = atof(optarg);
					printf("y: %f\n", y_arg);
					var_init|=0b0000010;
					break;
				case 't': // motor channel option
					theta_arg = atof(optarg);
					printf("theta: %f\n", theta_arg);
					var_init|=0b0000100;
					break;
				case 'm': // motor channel option
					mode_arg = atoi(optarg);
					printf("mode: %d\n", mode_arg);
					var_init|=0b0001000;
					break;
				case 'r': // motor channel option
					absolute = false;
					printf("coordinates relative\n");
					break;
				case 'G': // motor channel option
					if(action_done==false)
					{
						action=go_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'T': // motor channel option
					if(action_done==false)
					{
						action=turn_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'S': // motor channel option
					if(action_done==false)
					{
						printf("on va utiliser le servo\n");
						action=servo_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'P': // motor channel option
					if(action_done==false)
					{
						action=position_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'E': // motor channel option
					if(action_done==false)
					{
						action=state_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'A': // motor channel option
					if(action_done==false)
					{
						action=add_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'L': // motor channel option
					if(action_done==false)
					{
						action=launch_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;

				case 'C': // motor channel option
					if(action_done==false)
					{
						action=erase_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'D': // motor channel option
					if(action_done==false)
					{
						action=pid_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'X': // motor channel option
					if(action_done==false)
					{
						action=speed_accel_set_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'Y': // motor channel option
					if(action_done==false)
					{
						action=set_pos_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'V': // motor channel option
					if(action_done==false)
					{
						action=get_speed_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				case 'B': // motor channel option
					if(action_done==false)
					{
						action=motor_pwm_;
						action_done=true;
					}
					else
					{
						action=none_;
						printf("trop d'actions demandées\n");
					}
					break;
				default:
					break;
				}
			}
//on applique les résultats
			char buffer[80];
			int len;

			switch (action){
			case servo_: // motor channel option
				printf("on va utiliser le servo\n");
				printf("print %f %f %f %d %d %d %d %d\n",x_arg,y_arg,theta_arg,(uint8_t) mode_arg,(uint8_t)var_init,(uint8_t) ~var_init,(uint8_t)~0b00001100,(uint8_t)(~var_init)&(0b00001100));
				if(((~var_init)&(0b00001100))!=0)
				{
					printf("mettre une valeur d'angle ou quel servo dans mode -m\n");
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ko", strlen("ko")+1);
					close(fd1);
					break;
				}
				else
				{
					switch (mode_arg)
					{
					case(0):
						servo0.set_pos(theta_arg);
						break;
					case(1):
						servo1.set_pos(theta_arg);
						break;
					case(2):
						servo2.set_pos(theta_arg);
						printf("on essaie le servo 2 à la valeur %f ", theta_arg);
						break;
					case(3):
						servo3.set_pos(theta_arg);
						break;
					case(4):
						servo4.set_pos(theta_arg);
						break;
					case(5):
						servo5.set_pos(theta_arg);
						break;
					case(6):
						servo6.set_pos(theta_arg);
						break;
					case(7):
						servo7.set_pos(theta_arg);
						break;
					default:
					break;
					}
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ok", strlen("ok")+1);
					close(fd1);
				}
				break;
			case go_: // motor channel option
				printf("print %f %f %f %d %d %d %d %d\n",x_arg,y_arg,theta_arg,(uint8_t) mode_arg,(uint8_t)var_init,(uint8_t) ~var_init,(uint8_t)~0b00001100,(uint8_t)(~var_init)&(0b00001100));
				printf("on va en x,y,theta, avec un mode particulier \n");
				if(((~var_init)&(0b00001111))!=0)
				{
					printf("indiquer x,y,theta,et mode (+1,0,-1 pour avant, indifferent ou arrière -m\n");
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ko", strlen("ko")+1);
					close(fd1);
					break;
				}
				else
				{
					if (absolute==false)
					{
						coordinates_t dest,add;
						dest = control.get_destination();
						add.x=x_arg;
						add.y=y_arg;
						add.theta=theta_arg;
						asserv.add_coordinate(&dest,dest,add);
						x_arg=add.x;
						y_arg=add.y;
						theta_arg=add.theta;
					}
					switch(mode_arg)
					{
					case -1:
						asserv.force_direction(force_backwardward);
						break;
					case 0:
						asserv.force_direction(asserv_any_dir);
						break;
					case 1:
						asserv.force_direction(force_forward);
						break;
					default:
						printf("not changing direction, wrong choice");
					}
					asserv.go_destination(x_arg,y_arg,theta_arg);					
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ok", strlen("ok")+1);
					close(fd1);
				}
				break;
			case turn_: // motor channel option
				printf("on va en x,y,theta, avec un mode particulier \n");
				if(((~var_init)&(0b00000100))!=0)
				{
					printf("indiquer theta\n");
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ko", strlen("ko")+1);
					close(fd1);
					break;
				}
				else
				{
					if(absolute==false)
						theta_arg+=position.theta();
					asserv.go_angle(theta_arg);					

					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ok", strlen("ok")+1);
					close(fd1);
				}
				break;
			case pid_: // motor channel option
			
				printf("on configure le pid\n");
				if(((~var_init)&(0b00001111))!=0)
				{
					printf("manque argument\n");
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ko", strlen("ko")+1);
					close(fd1);
					break;
				}
				else
				{
					switch (mode_arg)
					{
					case 0:
						control.set_pid_speed(x_arg,y_arg,theta_arg);
						break;
					case 1:
						control.set_pid_dist(x_arg,y_arg,theta_arg);
						break;
					default:
						break;
					}
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ok", strlen("ok")+1);
					close(fd1);
				}
				break;
			case speed_accel_set_: // motor channel option
			
				printf("on configure le pid\n");
				if(((~var_init)&(0b00001011))!=0)
				{
					printf("manque argument\n");
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ko", strlen("ko")+1);
					close(fd1);
					break;
				}
				else
				{
					switch (mode_arg)
					{
					case 0:
						control.set_speed_accel(x_arg,y_arg);
						break;
					case 1:
						control.set_rot_speed_accel(x_arg,y_arg);
						break;
					default:
						break;
					}
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ok", strlen("ok")+1);
					close(fd1);
				}
				break;			
			case position_: // motor channel option
			
				len= sprintf (buffer, "%f %f %f", position.x(),position.y(),position.theta());
				fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
				write(fd1,buffer,len);
				close(fd1);
				break;

			case set_pos_: // motor channel option
			
				if(var_init&(0b00000001)!=0)
					position.update_x(x_arg);
				if(var_init&(0b00000010)!=0)
					position.update_y(y_arg);
				if(var_init&(0b00000100)!=0)
					position.update_theta(theta_arg);
				break;			
			case get_speed_: // motor channel option
				len= sprintf (buffer, "%f %f", position.speed(left_t),position.speed(right_t));
				fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
				write(fd1,buffer,len);
				close(fd1);
				break;

			case motor_pwm_: // motor channel option
			
				printf("on configure le pid\n");
				if(((~var_init)&(0b00001000))!=0)
				{
					printf("manque argument\n");
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ko", strlen("ko")+1);
					close(fd1);
					break;
				}
				else
				{
					switch (mode_arg)
					{

					case -1:
						asserv.motor_unlock();
						break;
					case 0:
						asserv.motor_lock();
						break;
					case 1:
					case 2:
					case 3:
					case 4:
						if(((~var_init)&(0b00001001))!=0)
						{
							printf("manque argument\n");
							fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
							write(fd1, "ko", strlen("ko")+1);
							close(fd1);
							break;
						}
						asserv.set_pwm(mode_arg, x_arg);
						break;
					default:
						break;
					}
					fd1 = open(myfifo_out,O_WRONLY|O_NONBLOCK);
					write(fd1, "ok", strlen("ok")+1);
					close(fd1);
				}
				break;			
			default:
				break;
			}
			printf("parsing finished");
		 //answer
			// if(strcmp(str1,"ok")<2)
			// {
				// fd1 = open(myfifo,O_WRONLY|O_NONBLOCK);
				// write(fd1, "ok", strlen("ok")+1);
				// close(fd1);

				// rc_usleep(100000);
			// }
		}
		rc_usleep(10000);

	}

	pthread_exit(NULL);
		
}


void Parser::parser_init(){
//	remove(myfifo);
	if (pthread_create(&parser_thread, NULL, parser_function, NULL)) {
		perror("pthread_create");
    }

}
Parser::~Parser(){
	pthread_join(parser_thread, NULL);
}
Parser::Parser(){
	;
}

