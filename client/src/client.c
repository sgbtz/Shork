// client/src/client.c

/*** APPLICATION FILES ***************/
#include "./client.h"
#include "./user/menu.c"
#include "./login.c"

/*** INCLUDES ************************/
#include <stdio.h>
#include <unistd.h>

/*** CLIENT MAIN PROCESS *************/
int main(void) {
	// Call login menu
	login();

	return 0;
}