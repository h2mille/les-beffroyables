#ifndef PARSER_H
#define PARSER_H

#include <queue>
#include <stack>
using namespace std;


#define myfifo "/tmp/robot_com"


static void *parser_function(void *arg);


class Parser{
    
    public:
	Parser();
	~Parser();
	void parser_init();




    
    private:

	pthread_t parser_thread;


};

#endif