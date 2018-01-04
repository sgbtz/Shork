#ifndef CLIENT_H
#define CLIENT_H


/*** DEFINITIONS *********************/
#define MAX_UNAME			16
#define MAX_PASS			16
#define MAX_FOLD_URL	64
#define MAX_USER_SIZE MAX_UNAME + MAX_PASS + MAX_FOLD_URL

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
	int ptail; // private tail ids
} LogRes;

typedef struct {
	long mtype; // REQ
	unsigned scope; // public/private
	unsigned cmd; // move/delete
	unsigned ud; // upload/download
	User user;
	char file[MAX_FOLD_URL];
	char fold[MAX_FOLD_URL];
} Opt;

typedef struct {
	long mtype; // RES
	unsigned error; // ERROR/OK
} Res;

/*** FUNCTIONS DEFINITIONS ****************/

// login.c
/*****************************************/
/*
** Ask user for username and pass and
** pass it to the server within a tail
** until the login successes
*/
void login();

// controllers/res_controller.c
/*****************************************/
/*
** Recive either the private or the
** shared tail, send within this tail
** a message to the server asking for
** a move action
** -----------------------------------
** Return within the tail the result
** of the operation
*/
void move(int tail, unsigned scope, User * user, unsigned ud);
/*
** Recive either the private or the
** shared tail, send within this tail
** a message to the server asking for
** a delete action
** -----------------------------------
** Return within the tail the result
** of the operation
*/
void delt(int tail, unsigned scope, User * user);
/*
** Recive the scope and the user
** shows the files either in the
** shared dir or the private one
*/
void show(unsigned scope, User * user);

// user/menu.c

/*Show the client's menu to the user. Need the id of the tail
** and a struct with the information of the userç
*/
void menu(int msgid, User *user);

#endif