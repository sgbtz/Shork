#ifndef SERVER_H
#define SERVER_H

#include "../lib/cjson/cJSON.h"
#include "../lib/cjson/cJSON.c"
#include <semaphore.h>

/*** DEFINITIONS *********************/
#define MAX_UNAME			16
#define MAX_PASS			16
#define MAX_FOLD_URL	64
#define TAM_MEMORY 		100
#define MAX_SEND_SIZE 200


/*** TYPE DEFINITIONS ****************/
typedef struct {
	char password[MAX_PASS];
	char user_name[MAX_UNAME];
	char user_folder[MAX_FOLD_URL];
} User;

typedef struct {
	char *name;
	unsigned status;
}File;

typedef struct {
	long mtype;
	char user_name[MAX_UNAME];
	char password[MAX_PASSWORD];
} LogReq;

typedef struct {
	long mtype;
	User user;
	unsigned error; // 0 - if error ; 1 - if not error
} LogRes;

/*** FUNCTIONS DEFINITIONS ****************/

// admin/menu.c
/*****************************************/
/*
** Admin menu, here the admin can create,
** delete or modify users
*/
void menu();
/*****************************************/

// controller/user_controller.c
/*****************************************/
/* 
** Recieves a User structure with a password and
** a username. Checks whether the user is valid and,
** if he is, gives him the user folder
** Return: -----------------------------
**	1 if the login is valid
**	0 otherwise
*/
unsigned auth_user(User * user);
/*
** Recives a User structure to be created in the
** user database (user.json)
** Return: -----------------------------
**	1 if the creation had been succesfully completed
** 	0 if the user couldn't be created
*/
unsigned create_user(User * user);
/*
** Recives a User structure to be deleted in the
** user database (user.json)
** Return: -----------------------------
**	1 if the deletion had been succesfully completed
** 	0 if the user couldn't be deleted
*/
unsigned delete_user(User * user);
/*
** Recives a User structure to be updated in the
** user database (user.json)
** Return: -----------------------------
**	1 if the update had been succesfully completed
** 	0 if the user couldn't be updated
*/
unsigned update_user(User * user);
/*
** Reads a file and parses its content to json and
** returns
** Return: -----------------------------
**	The json structure
**	NULL if there was an error
*/
cJSON * get_json();
/*****************************************/

// resources.c
/*****************************************/
/*Recive the number of users can be connect at the same time.
** Return if an error was happened.
** 0 no error
** 1 error openning semaphore
** 2 error creating share memory
** 3 error creating tail
*/
int init(int users);

/*This function delete all the resources which the programme initialize.
** Return an error happened
** 0 no error
** 1 error closing semaphore
** 2 error deleting share memory
** 3 error deleting tail
*/

int end();

/*Create a tail and return his ID*/

int co_cola(key_t clave);

/*Create a semaphore and return his ID*/
sem_t *co_sem(char* name, int tam);


/*Create a share memory and return his ID*/
int co_mm(key_t clave, int tam);

 /*Delete a tail with his msgid ID and return 3 if an error happened*/
int dt_c(int msgid);


/*Delete a share memory with his shmid ID and return 2 if an error happened*/
int dt_mm(int shmid);


/*Map the share memory with all the files in the route since the variable i. Return the share memory mapped*/
File *map_folder(char* route, int shmid, int i);

/*Search if the file with the same name that "name" is free and return the status*/
int used(char *name, File *file);
/******************************************/

// admin/menu.c
/*****************************************/
/*
** Admin menu, here the admin can create,
** delete or modify users
*/
void menu();
/*****************************************/

// controller/log_controller.c
/*****************************************/
/*
** Wait for login request recived within the share tail
** auth the user and return a response
** -----------------------------------
** Return within the tail "username":
** - "username -1" if failed
** - "username /userfolder" if correct
*/
void login();
/*
** Recive a user an create a connection
** if the parameters are ok.
** ------------------------------------
** Send a message to client:
**  - OK if there have been no error
**	- ERROR if auth failed
*/
void * connection(User * user);
/*****************************************/

#endif