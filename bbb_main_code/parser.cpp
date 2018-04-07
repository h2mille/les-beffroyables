
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "parser.h"
#include <pthread.h>

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

- regler vitesse angulaire
- regler acceleration angulaire
- regler vitesse lineaire
- regler acceleration lineaire

- regler PID

*/

enum action_t
{
	go_,
	turn_,
	servo_,
	position_,
	state_,
	add_,
	lauch_,
	erase_
};


typedef struct {
	float value;
	bool init;
}init_value_t;


	

	
	
	
	
Parser parser;



	


static void *parser_function(void *arg){ 
	umask(0000);
    mkfifo(myfifo, 0666);
	int fd1;
	fd_set fds;
	timeval tv ={0,100};//(sec,usec)
	int size,i;


	char str1[80];
	char *argv[80];
	int argc=0;
	
	float x_arg;
	float y_arg;
	float theta_arg;
	float mode_arg;
	
	argv[0]=&str1[0];
	argc++;
	while(rc_get_state() != EXITING){
		fd1 = open(myfifo,O_RDONLY|O_NONBLOCK);		
		size = read(fd1, str1, 80);
		close(fd1);
		if(size>0)
		{
//on nettoie la chaine
			close(fd1);
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
			init_value_t value[4];
			value[0].init=false;
			value[1].init=false;
			value[2].init=false;
			value[3].init=false;
			bool action_done=false;
			action_t action;
			bool absolute= true;
			
		 	while ((c = getopt(argc, argv, "x:y:t:m:rGTS::PEALE")) != -1){
				switch (c){
				case 'x': // motor channel option
					value[0].value = atof(optarg);
					printf("x: %f\n", value[0].value);
					value[0].init=true;
					break;
				case 'y': // motor channel option
					value[1].value = atof(optarg);
					printf("y: %f\n", value[1].value);
					value[1].init=true;
					break;
				case 't': // motor channel option
					value[2].value = atof(optarg);
					printf("theta: %f\n", value[2].value);
					value[2].init=true;
					break;
				case 'm': // motor channel option
					value[3].value = atof(optarg);
					printf("mode: %f\n", value[3].value);
					value[3].init=true;
					break;
				case 'r': // motor channel option
					absolute = false;
					printf("coordinates relative\n");
					break;
				case 'S': // motor channel option
					if(value[2].init== false)
					{
						printf("mettre une valeur d'angle\n");
						break;
					}else if(optarg==NULL)
					{
						rc_send_servo_pulse_normalized_all(value[2].value);
						break;
					}else
					{
						rc_send_servo_pulse_normalized(atoi(optarg), value[2].value);
					}
					break;
				default:
					break;
				}
			}
//on applique les résultats
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

