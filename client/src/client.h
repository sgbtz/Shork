#ifndef CLIENT_H
#define CLIENT_H


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
	char password[MAX_PASS];
} LogReq;

typedef struct {
	long mtype;
	User user;
	unsigned error; // 0 - if error ; 1 - if not error
} LogRes;

#endif