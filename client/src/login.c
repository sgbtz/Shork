// client/src/client.c

/*** APPLICATION FILES ***************/
#include "./client.h"

/*** INCLUDES ************************/
#include <stdio.h>
#include <unistd.h>

/*** DEFINES *************************/
#define REQ 1 // Login request will be type 1
#define RES 2 // Login response will be type 2
#define ERROR 0
#define OK 1
#define MAX_REQ_SIZE MAX_UNAME + MAX_PASS + sizeof(long) // Max size of the request msg
#define MAX_RES_SIZE MAX_UNAME + MAX_PASS + MAX_FOLD_URL + sizeof(long) + sizeof(unsigned) // MAx size of the response msg

/*** FUNCTIONS ***********************/
/*
** Ask user for username and pass and
** pass it to the server within a tail
** until the login successes
*/
void login() {
	int tail = 0; // users tail id
	key_t key; // key for the tail
	User * user;
	LogReq * req; // request goes here
	LogRes * res; // response goes here
	success = ERROR;

	key = ftok("cola",'P');
	tail = co_cola(key);

 	// Ask for username and pass while they are incorrect
	while(!success) {
		printf("Username: ");
		scanf("%s", req->user_name);
		printf("\nPassword: ");
		scanf("%s", req->password);
		req->mtype = REQ;
		// Send the request
		msgsend(tail, req, MAX_RES_SIZE, 0)
		// Wait for the response to the correct user
		do {
			msgrcv(tail, res, MAX_REQ_SIZE, REQ, 0);
		} while(!strcmp(req->user_name, res->user.user_name));
		// Checks the result
		if (res->error) {
			success = OK;
		}
	}

}