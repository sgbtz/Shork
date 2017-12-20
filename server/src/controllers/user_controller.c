// server/src/controllers/user_controller.c

/*** APPLICATION FILES ***************/
#include "../server.h"
#include "./json_controller.c"

/*** INCLUDES ************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*** DEFINES *************************/
#define USERS_URL			"../res/users.json"
#define ERROR_USER		"ERROR: User %s not found\n"
#define ERROR 				0
#define OK						1

/*** FUNCTIONS ***********************/
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
	unsigned success = OK;

	// Get structure of users
	if (!(users = get_json(USERS_URL)))
		success = ERROR;
	else if (!(juser  = cJSON_GetObjectItemCaseSensitive(users, user->user_name))) {
		cJSON_Delete(users); // close json object
		success = ERROR;
	} // Check password
	else {
		pass = cJSON_GetObjectItemCaseSensitive(juser, "password");
		if (strcmp(user->password, pass->valuestring))
			success = ERROR; // wrong password
		else { // successful login
			folder = cJSON_GetObjectItemCaseSensitive(juser, "folderURL");
			strncpy(user->user_folder, folder->valuestring, MAX_PASS); // gives user personal folder
		}

		cJSON_Delete(users); // close json object
	}

	return success;
}

/*
** Recives a User structure to be created in the
** user database (user.json)
** Return: -----------------------------
**	1 if the creation had been succesfully completed
** 	0 if the user couldn't be created
*/
unsigned create_user(User * user){
	cJSON * users = NULL;
	cJSON * juser = NULL;
	unsigned success = OK;

	// get structure of users
	if (!(users = get_json(USERS_URL)))
		success = ERROR;
	else { // create user item
		cJSON_AddItemToObject(users, user->user_name, juser = cJSON_CreateObject());
		cJSON_AddStringToObject(juser, "folderURL", user->user_folder);
		cJSON_AddStringToObject(juser, "password", user->password);
		if(!(set_json(cJSON_Print(users), USERS_URL))) // modify json database
			success = ERROR;

		cJSON_Delete(users); // close json object
	}

	return success;
}

/*
** Recives a User structure to be deleted from the
** user database (user.json)
** Return: -----------------------------
**	1 if the deletion had been succesfully completed
** 	0 if the user couldn't be deleted
*/
unsigned delete_user(User * user){
	cJSON * users = NULL;
	cJSON * juser = NULL;
	unsigned success = OK;

	// get structure of users
	if (!(users = get_json(USERS_URL)))
		success = ERROR;
	else if (!(juser  = cJSON_GetObjectItemCaseSensitive(users, user->user_name))) {
		cJSON_Delete(users); // close json object
		success = ERROR;
	}
	else { // delete user item
		cJSON_DeleteItemFromObjectCaseSensitive(users, user->user_name);
		if(!(set_json(cJSON_Print(users), USERS_URL))) // modify json database
			success = ERROR;

		cJSON_Delete(users); // close json object
	}

	return success;
}

/*
** Recives a User structure to be updated in the
** user database (user.json)
** Return: -----------------------------
**	1 if the update had been succesfully completed
** 	0 if the user couldn't be updated
*/
unsigned update_user(User * user){
	cJSON * users = NULL;
	cJSON * juser = NULL;
	cJSON * nuser = NULL;
	cJSON * npass = NULL;
	cJSON * nfold = NULL;
	unsigned success = OK;

	// get structure of users
	if (!(users = get_json(USERS_URL)))
		success = ERROR;
	else if (!(juser  = cJSON_GetObjectItemCaseSensitive(users, user->user_name))) {
		cJSON_Delete(users); // close json object
		success = ERROR;
	}
	else { // update user item
		// Create cJSON objects to update the user
		npass = cJSON_CreateString(user->password);
		nfold = cJSON_CreateString(user->user_folder);
		// Add created objects to user to be updated
		cJSON_AddItemToObject(users, user->user_name, juser = cJSON_CreateObject());
		cJSON_AddStringToObject(juser, "folderURL", user->user_folder);
		cJSON_AddStringToObject(juser, "password", user->password);
		// Replace old user with new
		cJSON_ReplaceItemInObjectViaPointer(users, juser, nuser);
		if(!(set_json(cJSON_Print(users), USERS_URL))) // modify json database
			success = ERROR;

		cJSON_Delete(users); // close json object
	}

	return success;
}