#ifndef SERVER_H
#define SERVER_H

#include "../lib/cjson/cJSON.h"
#include "../lib/cjson/cJSON.c"

/*** DEFINITIONS *********************/
#define MAX_UNAME			16
#define MAX_PASS			16
#define MAX_FOLD_URL		64

/*** TYPE DEFINITIONS ****************/
typedef struct User {
	char password[MAX_PASS];
	char user_name[MAX_UNAME];
	char user_folder[MAX_FOLD_URL];
} User;

/*** FUNCTIONS DEFINITIONS ****************/

// user_controller.c
/*****************************************/
/* 
** Recieves a User structure with a password and
** a username. Checks whether the user is valid and,
** if he is, gives him the user folder
*/
unsigned auth_user(User * user);
/* 
** Reads a file and parses its content to json and
** returns it 
*/
cJSON * get_json();
/*****************************************/

//#include "./controllers/user_controller.c"

#endif