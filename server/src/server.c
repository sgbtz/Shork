// server/src/server.c

/*** APPLICATION FILES ***************/
#include "./server.h"
#include "./admin/menu.c"
#include "./resources.c"
#include "./controllers/log_controller.c"
//#include "./controllers/th_controller.c"

/*** INCLUDES ************************/
#include <stdio.h>
#include <unistd.h>

/*** SERVER MAIN PROCESS *************/
int main(void) {
	pid_t pid = -1; // child/parent process id

	// Create the needed resources
	init();
	
	// Create a child process to manage the requests
	pid = fork();

	switch(pid) {
		case -1: // there had been an error in fork
			printf("Error during the execution\n");
			break;
		case 0: // child process
			menu();
			break;
		default: // parent process
			login();
			break;
	}

	return 0;
}