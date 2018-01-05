// client/src/controllers/res_controller.c
 
/*** APPLICATION FILES ***************/
#include "../client.h"

/*** INCLUDES ************************/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/wait.h>

/*** DEFINES *************************/
#define REQ 1 // Request will be type 1
#define RES 2 // Response will be type 2
#define ERROR 0
#define OK 1
#define MAX_OPT_SIZE MAX_USER_SIZE + 2*MAX_FOLD_URL + sizeof(long) + 2*sizeof(unsigned)
#define MAX_RES_SIZE sizeof(long) + sizeof(unsigned)
#define MOVE 1
#define DEL  2
#define PUBLIC 1
#define PRIVATE 2
#define URL_PUBLIC "server/res/share"

/*** FUNCTIONS ***********************/
/*
** Recive either the private or the
** shared tail, send within this tail
** a message to the server asking for
** a move action
** -----------------------------------
** Return within the tail the result
** of the operation
*/
void move(int tail, unsigned scope, User * user, unsigned ud) {
	Opt * opt = malloc(MAX_OPT_SIZE);
	Res * res = malloc(MAX_RES_SIZE);
	char file[MAX_FOLD_URL];
	char chw[MAX_FOLD_URL];
	// Ask for a file to be moved
	printf("What file do you want to move?: ");
	scanf("%s", file);
	// Set parameters of the struct
	opt->mtype = REQ;
	opt->scope = scope;
	opt->cmd = MOVE;
	opt->ud = ud;
	opt->user = *user;
	strcpy(opt->file, file);
	strcpy(opt->fold, getcwd(chw, MAX_FOLD_URL));
	// Send message
	msgsnd(tail, opt, MAX_OPT_SIZE, 0);
	// Wait for a response
	msgrcv(tail, res, MAX_RES_SIZE, RES, 0);
	// Check if the operation had success
	if (res->error)
		printf("Operation MOVE couldn't be completed!\n");
	else
		printf("Operation MOVE completed successfully!\n");
}

/*
** Recive either the private or the
** shared tail, send within this tail
** a message to the server asking for
** a delete action
** -----------------------------------
** Return within the tail the result
** of the operation
*/
void delt(int tail, unsigned scope, User * user) {
	Opt * opt = malloc(MAX_OPT_SIZE);
	Res * res = malloc(MAX_RES_SIZE);
	char file[MAX_FOLD_URL];
	// Ask for a file to be deleted
	printf("What file do you want to delete?: ");
	scanf("%s", file);
	// Set parameters of the struct
	opt->mtype = REQ;
	opt->scope = scope;
	opt->cmd = DEL;
	strcpy(opt->file, file);
	opt->user = *user;
	opt->ud = 0;
	// Send message
	msgsnd(tail, opt, MAX_OPT_SIZE, 0);
	// Wait for a response
	msgrcv(tail, res, MAX_RES_SIZE, RES, 0);
	// Check if the operation had success
	if (res->error)
		printf("Operation DELETE couldn't be completed!\n");
	else
		printf("Operation DELETE completed successfully!\n");
}
/*
** Recive the scope and the user
** shows the files either in the
** shared dir or the private one
*/
void show(unsigned scope, User * user) {
	pid_t pid = -1; // child/parent process id
	int value;
	char dest[MAX_FOLD_URL];
	// Create a child process to manage the requests
	pid = fork();
	if (scope == PUBLIC)
		strcpy(dest, URL_PUBLIC);
	else
		strcpy(dest, user->user_folder);

	switch(pid) {
		case -1: // there had been an error in fork
			printf("Error during the execution\n");
			break;
		case 0: // child process
			if(execlp("ls", "ls", dest, NULL) < 0)
				printf("Error showing files\n");
			break;
		default: // parent process
			wait(&value);
			break;
	}
}