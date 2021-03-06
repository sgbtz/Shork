// server/src/controllers/log_controller.c

/*** APPLICATION FILES ***************/
#include "../server.h"
#include "./user_controller.c"
#include "./th_controller.c"

/*** INCLUDES ************************/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>

/*** DEFINES *************************/
#define REQ 1 // Login request will be type 1
#define RES 2 // Login response will be type 2
#define ERROR 0
#define OK 1
#define MAX_THREADS 10
#define MAX_LOGREQ_SIZE MAX_UNAME + MAX_PASS + sizeof(long) // Max size of the request msg
#define MAX_LOGRES_SIZE MAX_USER_SIZE + sizeof(long) + sizeof(unsigned) // MAx size of the response msg

/*** TYPE DEFINITIONS ****************/
typedef struct {
	int tail;
	User user;
} Conn;

/*** FUNCTIONS ***********************/
/* Handle signal */
void end_handle() {
	pid_t pid;
	end();
	pid = getpid();
	kill(pid,SIGKILL); //send signal to this process.
}
/*
** Wait for login request recived within the share tail
** auth the user and return a response in a thread
*/
void login() {
	int tail = 0, i = 0; // users tail id and counter
	struct sigaction act; // struct for signal
	key_t key; // key for the tail
	LogReq * req = malloc(MAX_LOGREQ_SIZE); // message goes here
	User * user = malloc(MAX_USER_SIZE);
	pthread_attr_t attr;
	pthread_t conn[MAX_THREADS];
	Conn * info = malloc(MAX_USER_SIZE + sizeof(int));
	// Signal handle 
	act.sa_handler = &end_handle;
	act.sa_flags= 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);

	key = ftok("server/res/share/.",'P');
	tail = co_cola(key);
	// Config attributes for threads
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	while(1){
		// Look for a login request in the tail
		if((MAX_LOGREQ_SIZE == msgrcv(tail, req, MAX_LOGREQ_SIZE, REQ, 0))) {
			strcpy(user->user_name, req->user_name);
			strcpy(user->password, req->password);
			// Create a thread to manage the client request
			info->tail = tail;
			info->user = *user;
			pthread_create(&conn[i], &attr, connection, info);
			i++;
		}
		pthread_attr_destroy(&attr);
	}
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
	LogRes * res = malloc(MAX_LOGRES_SIZE);
	Conn * conn = (Conn *) con;
	int ptail = 0;
	key_t pkey = 0;
	char folder[MAX_FOLD_URL];
	// Checks if the parameters are OK
	if (!auth_user((&conn->user))) { // if not
		res->user = conn->user;
		res->error = ERROR;
		res->mtype = RES;
		msgsnd(conn->tail, res, MAX_LOGRES_SIZE, 0); // return an error
	} else { // if yes
		// Concatenate folder url to create private tail
		strcat(folder,FOLDER_URL);
		strcat(folder,conn->user.user_name);
		// Generate key and tail
		pkey = ftok(folder,'A');
		ptail = co_cola(pkey);
		// Pass parameters to the user
		res->ptail = ptail;
		res->user = conn->user;
		res->error = OK;
		res->mtype = RES;
		msgsnd(conn->tail, res, MAX_LOGRES_SIZE, 0); // return success
		th_controller(ptail);
	}

	pthread_exit(NULL);
}