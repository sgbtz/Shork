/***INCLUDES****/
#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h> 
#include <string.h>
#include <semaphore.h>


/***APPLICATION FILES**/
#include "../server.h"
#include "./file_controller.c"


/***DEFINES***/
#define MAX_OPT_SIZE MAX_USER_SIZE + 2*MAX_FOLD_URL + sizeof(long) + 2*sizeof(unsigned)
#define MAX_RES_SIZE sizeof(long) + sizeof(unsigned)
#define ACTION 2
#define ISPRIVATE 2
#define ISPUBLIC 1
#define MOVE 1
#define DELETE 2
#define RC 1
#define UPLOAD 1
#define REQ 1
#define RES 2
#define END 3
#define DEST_PUBLIC_PATH "server/res/share/"

/***TYPEDEFS******/
typedef struct{ 
	long mtype;
	unsigned scope;
	unsigned cmd;
	unsigned ud;
	User user;
	char file[MAX_FOLD_URL];
	char fold[MAX_FOLD_URL];
}Opt;

typedef struct {
	long mtype;
	unsigned error;
}Res;


void th_controller(int ptail){

	Opt * opt = malloc(MAX_OPT_SIZE);/*Queue buffer*/
	Res * res = malloc(MAX_RES_SIZE);
	File * file;
	key_t clavem;
	sem_t *mutex=NULL;
	int shmid;
	char org[MAX_FOLD_URL];
	char dest[MAX_FOLD_URL];
	int free = 0;
	int end = 0;
	res->error = 1;
	int * nfiles = malloc(sizeof(int));
	*nfiles = 0;

	clavem=ftok("server/res/share/.",'A');
	shmid = co_mm(clavem,TAM_MEMORY);
	mutex = sem_open("mutex",0);
	/*map the memory*/
	if((file =shmat(shmid,NULL,0)) == (File *)-1)
		printf("Error al mapear la memoria compartida\n");
	else{
		map_folder(DEST_PUBLIC_PATH, shmid, nfiles);
		while(!end) {
			msgrcv(ptail, opt, MAX_OPT_SIZE,REQ,0);
			if(opt->cmd != END) {
				/*depending on the type of the msg*/
				if(opt->scope==ISPRIVATE){
					switch(opt->cmd) {
						/*MOVE FILE*/
						case MOVE:
							if (!opt->ud) { // if download
								strcpy(org, opt->user.user_folder);
								strcat(org, "/");
								strcat(org, opt->file);
								strcpy(dest, opt->fold); // move to downloads folder
								strcat(dest, "/");
								strcat(dest, "downloads");
								strcat(dest, "/");
								strcat(dest, opt->file);
							} else { // if upload
								strcpy(dest, opt->user.user_folder);
								strcat(dest, "/");
								strcat(dest, opt->file);
								strcpy(org, opt->fold); // move from uploads folder
								strcat(org, "/");
								strcat(org, "uploads");
								strcat(org, "/");
								strcat(org, opt->file);
							}
							res->error = move(org,dest);
					 	break;

					 	/*DELETE FILE*/
					 	case DELETE:
							strcpy(org, opt->user.user_folder);
							strcat(org, "/");
							res->error = dlt(org, opt->file);
					 	break;
					}

				}
					
				else if(opt->scope==ISPUBLIC) {

					if (!opt->ud) { // if download
						sem_wait(mutex);
						free = used(opt->file, file);
						sem_post(mutex);
						strcpy(org, DEST_PUBLIC_PATH);
						strcat(org, opt->file);
						strcpy(dest, opt->fold); // move to downloads folder
						strcat(dest, "/");
						strcat(dest, "downloads");
						strcat(dest, "/");
						strcat(dest, opt->file);
					} else { // if upload
						strcpy(dest, DEST_PUBLIC_PATH);
						strcat(dest, opt->file);
						strcpy(org, opt->fold); // move from uploads folder
						strcat(org, "/");
						strcat(org, "uploads");
						strcat(org, "/");
						strcat(org, opt->file);
					}

					if((free && !opt->ud) || opt->ud) {
						switch(opt->cmd) {
							/*MOVE FILE*/
							case MOVE:
								if(opt->ud && ((*nfiles-3) >= MAX_FILES)) {
									res->error = 10;
								}
								else
									res->error = move(org,dest);								
								break;
							/*DELETE FILE*/
							case DELETE:
								res->error = dlt(DEST_PUBLIC_PATH, opt->file);
								break;
						}

						*nfiles = 0;
						sem_wait(mutex);
						map_folder(DEST_PUBLIC_PATH, shmid, nfiles);
						sem_post(mutex);
					}
				}
				res->mtype = RES;
				msgsnd(ptail, res, MAX_RES_SIZE, 0);
			}
			else
				end = 1;
		}
	}
}




