// client/src/controllers/res_controller.c

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
#define MOVE "move"
#define DEL "delete"
#define FOLDER_URL		"../res/users/"

/*** FUNCTIONS ***********************/
void move(int tail) {

}

/**/
void delete(int tail) {

}