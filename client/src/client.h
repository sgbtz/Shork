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
} LogRes;


/*Show the client's menu to the user. Need the id of the tail
** and a struct with the information of the user*/
void menu(int msgid, User *user);

#endif