// server/src/controllers/log_controller.c

/*** APPLICATION FILES ***************/
#include "./server.h"
#include "../resources.c"
#include "./th_controller.c"

/*** INCLUDES ************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/msg.h>

/*** DEFINES *************************/
#define REQ 1 // Login request will be type 1
#define RES 2 // Login response will be type 2
#define ERROR 0
#define OK 1
#define MAX_THREADS 10
#define MAX_REQ_SIZE MAX_UNAME + MAX_PASSWORD + sizeof(long) // Max size of the request msg
#define MAX_RES_SIZE MAX_UNAME + MAX_PASSWORD + MAX_FOLD_URL + sizeof(long) + sizeof(unsigned) // MAx size of the response msg

/*** FUNCTION ************************/
/*
** Wait for login request recived within the share tail
** auth the user and return a response
** -----------------------------------
** Return within the tail "username":
** - "username -1" if failed
** - "username /userfolder" if correct
*/
void login() {
	int tail = 0, i = 0; // users tail id and counter
	key_t key; // key for the tail
	LogReq * req; // message goes here
	User * user;
	pthread_attr_t attr;
	pthread_t conn[MAX_THREADS];

	key = ftok("cola",'P');
	tail = co_cola(key);
	// Config attributes for threads
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	while(1){
		// Look for a login request in the tail
		if((MAX_REQ_SIZE == msgrcv(tail, req, MAX_REQ_SIZE, REQ, 0))) {
			strcpy(user->user_name, req->user_name);
			strcpy(user->password, req->password);
			// Create a thread to manage the client request
			pthread_create(&thid[i], &attr, connection, user);
			i++;
		}
	}

	pthread_attr_destroy(&attr);
}

/*
** Recive a user an create a connection
** if the parameters are ok.
** ------------------------------------
** Send a message to client:
**  - OK if there have been no error
**	- ERROR if auth failed
*/
void * connection(User * user) {
	LogRes * res;

	// Checks if the parameters are OK
	if (!auth(user)) { // if not
		res->user = *user;
		res->error = ERROR;
		res->mtype = RES;
		msgsnd(tail, res, MAX_RES_SIZE, 0); // return an error
	} else { // if yes
		res->user = *user;
		res->error = OK;
		res->mtype = RES;
		msgsnd(tail, res, MAX_RES_SIZE, 0); // return success
	}
}