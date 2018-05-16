#ifndef PARSER_H
#define PARSER_H

#include <queue>
#include <stack>
using namespace std;


#define myfifo_in "/tmp/robot_com_in"
#define myfifo_out "/tmp/robot_com_out"


static void *parser_function(void *arg);
static void *parser_listener(void *arg);


class Parser{
    
    public:
	Parser();
	~Parser();
	void parser_init();




    
    private:

	pthread_t parser_thread;


};

#endif