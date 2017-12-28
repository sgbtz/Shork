// server/src/controllers/log_controller.c

/*** APPLICATION FILES ***************/
#include "../server.h"
#include "./user_controller.c"
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
#define MAX_REQ_SIZE MAX_UNAME + MAX_PASS + sizeof(long) // Max size of the request msg
#define MAX_RES_SIZE MAX_USER_SIZE + sizeof(long) + sizeof(unsigned) // MAx size of the response msg

/*** TYPE DEFINITIONS ****************/
typedef struct {
	int tail;
	User user;
} Conn;

/*** FUNCTIONS ***********************/
/*
** Wait for login request recived within the share tail
** auth the user and return a response in a thread
*/
void login() {
	int tail = 0, i = 0; // users tail id and counter
	key_t key; // key for the tail
	LogReq * req = malloc(MAX_REQ_SIZE); // message goes here
	User * user = malloc(MAX_USER_SIZE);
	pthread_attr_t attr;
	pthread_t conn[MAX_THREADS];
	Conn * info = malloc(MAX_USER_SIZE + sizeof(int));

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
			info->tail = tail;
			info->user = *user;
			pthread_create(&conn[i], &attr, connection, info);
			i++;
		}
	}

	pthread_attr_destroy(&attr);
}

/*
** Recive a user an create a connection
** if the parameters are ok.
** ------------------------------------
** Return within the tail:
**  - OK if there have been no error
**	- ERROR if auth failed
*/
void * connection(void * con) {
	LogRes * res = malloc(MAX_RES_SIZE);
	Conn * conn = (Conn *) con;
	// Checks if the parameters are OK
	if (!auth_user(&(conn->user))) { // if not
		res->user = conn->user;
		res->error = ERROR;
		res->mtype = RES;
		msgsnd(conn->tail, res, MAX_RES_SIZE, 0); // return an error
	} else { // if yes
		res->user = conn->user;
		res->error = OK;
		res->mtype = RES;
		msgsnd(conn->tail, res, MAX_RES_SIZE, 0); // return success
	}

	pthread_exit(NULL);
}