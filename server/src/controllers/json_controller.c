// server/src/controllers/json_controller.c

/*** APPLICATION FILES ***************/

/*** INCLUDES ************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*** DEFINES *************************/
#define ERROR_READ		"ERROR: No such file or directory: %s\n"

/*
** Reads a file and parses its content to json and
** returns it 
*/
cJSON * get_json(char * url) {
	FILE * json_file = NULL;
	char * json_string = NULL; // Pre-parsed json 
	cJSON * root = NULL; // All json goes here
	int fsize = 0;   // json file size

	// Open json file
	if (!(json_file = fopen(url, "r")))
		printf(ERROR_READ, url);
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

		free(json_string); // free dynamic array
		fclose(json_file); // close json file
	}

	return root; // return json
}

/*
** Write new content in a file from json
*/
unsigned set_json(char * json_string, char * url) {
	FILE * json_file = NULL;
	unsigned jsize = 0;
	unsigned success = 1;

	jsize = strlen(json_string)*sizeof(char); // size of the json content
	// Open json file
	if (!(json_file = fopen(url, "w")))
		printf(ERROR_READ, url);
	else {
		// Write the json in the database file
		if(!(jsize == fwrite(json_string, 1, jsize, json_file)))
			success = 0; // content not written

		fclose(json_file); // close json file
	}

	return success; // return json
}