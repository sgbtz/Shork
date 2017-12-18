// server/src/controllers/user_controller.c

/*** APPLICATION FILES ***************/
#include "../server.h"

/*** INCLUDES ************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*** DEFINES *************************/
#define USERS_URL			"../res/users.json"
#define ERROR_READ		"ERROR: No such file or directory: %s\n"
#define ERROR_USER		"ERROR: User %s not found\n"
#define ERROR 				0
#define CORRECT				1

/*** FUNCTIONS ***********************/
/*
** Reads a file and parses its content to json and
** returns it 
*/
cJSON * get_json() {
	FILE * json_file = NULL;
	char * json_string = NULL; // Pre-parsed json 
	cJSON * root = NULL; // All json goes here
	int fsize = 0; // json file size

	// Open json file
	if (!(json_file = fopen(USERS_URL, "r")))
		printf(ERROR_READ, USERS_URL);
	else {
		// Getting file size for malloc
		fseek(json_file, 0L, SEEK_END); // going to end of the file
		fsize = ftell(json_file);
		rewind(json_file); // return to the beginning of the file
		
		json_string = (char *) malloc(sizeof(char)*fsize);
		// Read from file all json in string format
		fread(json_string, sizeof(char), fsize, json_file);
		// Parse json
		root = cJSON_Parse(json_string);
	}

	return root; // return json
}

/* 
** Recieves a User structure with a password and
** a username. Checks whether the user is valid and,
** if he is, gives him the user folder
*/
unsigned auth_user(User * user){
	cJSON * users  = NULL;
	cJSON * juser  = NULL;
	cJSON * pass   = NULL;
	cJSON * folder = NULL;
	unsigned success = CORRECT;

	// get structure of users and check password
	if (!(users = get_json()))
		success = ERROR;
	else if (!(juser  = cJSON_GetObjectItemCaseSensitive(users, user->user_name))) {
		printf(ERROR_USER, user->user_name); // wrong username
		success = ERROR;
	} 
	else {
		pass = cJSON_GetObjectItemCaseSensitive(juser, "password");
		if (strcmp(user->password, pass->valuestring))
			success = ERROR; // wrong password
		else { // successful login
			folder = cJSON_GetObjectItemCaseSensitive(juser, "folderURL");
			strncpy(user->user_folder, folder->valuestring, MAX_PASS); // gives user personal folder
		}
	}

	return success;
}