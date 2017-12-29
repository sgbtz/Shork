// server/src/server.c

/*** APPLICATION FILES ***************/
#include "./server.h"
#include "./admin/menu.c"
#include "./resources.c"
#include "./controllers/log_controller.c"
#include "./controllers/th_controller.c"

/*** INCLUDES ************************/
#include <stdio.h>
#include <unistd.h>

/*** SERVER MAIN PROCESS *************/
int main(void) {
	pid_t pid = -1; // child/parent process id
	pthread_attr_t attr;
	pthread_t thread;
	Conn * info = malloc(MAX_USER_SIZE + sizeof(int));

	// Create the needed resources
	init();
	// Create a child process to manage the requests
	pid = fork();

	switch(pid) {
		case -1: // there had been an error in fork
			printf("Error during the execution\n");
			break;
		case 0: // child process
			login()
			/*Set atributes to the thread*/
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

			/*Creat thread that execute th_controller function*/
			pthread_creat(thread,&attr,th_controller,info);

			break;
		default: // parent process
			menu();
			break;
	}

	return 0;
}