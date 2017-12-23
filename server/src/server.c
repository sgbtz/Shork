// server/src/server.c

/*** APPLICATION FILES ***************/
#include "./server.h"
#include "./admin/menu.c"

/*** INCLUDES ************************/
#include <stdio.h>
#include <unistd.h>

/*** SERVER MAIN PROCESS *************/
int main(void) {
	pid_t pid = -1; // child/parent process id

	// Create the needed resources
	init();
	/* to-do: check resources.c and put init() here */
	// Create a child process to manage the requests
	pid = fork();

	switch(pid) {
		case -1: // there had been an error in fork
			printf("Error during the execution\n");
			break;
		case 0: // child process
			/* to-do: function from req_controller.c goes here */
			break;
		default: // parent process
			menu();
			break;
	}

	return 0;
}