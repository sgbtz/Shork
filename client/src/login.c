// client/src/client.c

/*** APPLICATION FILES ***************/
#include "./client.h"

/*** INCLUDES ************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>

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
	User * user = malloc(MAX_USER_SIZE);
	LogReq * req = malloc(MAX_REQ_SIZE); // request goes here
	LogRes * res = malloc(MAX_RES_SIZE); // response goes here
	unsigned success = ERROR;

	key = ftok("../../server/res/share/.",'P');
	tail = msgget(key, IPC_CREAT);

 	// Ask for username and pass while they are incorrect
	while(!success) {
		printf("Username: ");
		scanf("%s", req->user_name);
		printf("\nPassword: ");
		scanf("%s", req->password);
		req->mtype = REQ;
		// Send the request
		msgsnd(tail, req, MAX_REQ_SIZE, 0);
		// Wait for the response to the correct user
		do {
			msgrcv(tail, res, MAX_RES_SIZE, RES, 0);
		} while(strcmp(req->user_name, res->user.user_name));
		// Checks the result
		if (res->error) {
			success = OK;
			*user = res->user;
			printf("OK\n");
		} else
			printf("ERROR\n");
	}

}