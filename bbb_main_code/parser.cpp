
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "parser.h"
#include "rc_usefulincludes.h"
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


#include "libraries/rc_usefulincludes.h"
#include "libraries/roboticscape.h"



/*
langage:
-x valeur en x
-y valeur en y
-t valeur en theta
-m mode de déplacement

-G aller à un point donné
-T tourner à un angle donné
-S Tourner le servo
-P afficher la postion
-E etat actuel de l'asservissement
-M regler PWM moteur

*/

Parser parser;




static void *parser_function(void *arg){ 
    mkfifo(myfifo, 0666);
	int fd1;
	fd_set fds;
	timeval tv ={0,100};//(sec,usec)
	int size,i;


	char str1[80];
	char *argv[80];
	int argc;
	
	while(rc_get_state() != EXITING){
		fd1 = open(myfifo,O_RDONLY|O_NONBLOCK);		
		size = read(fd1, str1, 80);
		close(fd1);
		if(size>0)
		{
			printf("User1: %s\r\n", str1);

			close(fd1);
			argc=0;
			for(i=0;i<size-1;i++)
			{
				printf("letter done %c\r\n", str1[i]);
				while(str1[i]==' ')
				{
					i++;
				printf("space found\r\n");			
				}
				if(i>0)
					str1[i-1]='\0';
				argv[argc]=&str1[i];
				printf("donc: emplcament argv : %d et adresse: %d\r\n",i,argv[argc]);
				argc++;
				while(str1[i]!=' ' && i!=size-1)
					i++;
			}
			while(argc!=0)
			{
				argc--;
				printf("%s\r\n",argv[argc]);
			}
		 
		 //answer
			// if(strcmp(str1,"ok")<2)
			// {
				// fd1 = open(myfifo,O_WRONLY|O_NONBLOCK);
				// write(fd1, "ok", strlen("ok")+1);
				// close(fd1);

				// rc_usleep(100000);
			// }
		}
	}
	rc_usleep(10000);
	pthread_exit(NULL);
		
}


void Parser::parser_init(){
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

