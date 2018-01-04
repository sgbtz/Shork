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
#define MAX_OPT MAX_USER_SIZE+MAX_FOLD_URL + sizeof(long)+2*sizeof(unsigned)
#define MAX_RES sizeof(long)+sizeof(unsigned)
#define MAX_FILENAME  20
#define ACTION 2
#define CD 3     /*change directory*/
#define ISPRIVATE 2
#define ISPUBLIC 1
#define MOVE 1
#define DELETE 2
#define RC 1
#define UPLOAD 1
#define REQ 1
#define RES 2
#define END -1
#define DEST_PUBLIC_PATH "../res/users/share/"

/***TYPEDEFS******/
typedef struct{ 
	long mtype;
	unsigned scope;
	unsigned cmd;
	unsigned ud;
	User user;
	char file[MAX_FILENAME];
	char fold[MAX_FILENAME];
}Opt;

typedef struct {
	long mtype;
	unsigned error;
}Res;


void th_controller(){

	key_t key;
	int queue_id = 0;
	Opt qbuffer;/*Queue buffer*/
	Res * res = malloc(MAX_RES);
	File * file;
	key_t clavem;
	sem_t *mutex=NULL;
	int shmid;
	char org[MAX_FOLD_URL];
	char dest[MAX_FOLD_URL];
	int free = 0;
	int end = 0;


	key=ftok("../res/share/.",'Q'); /*Creating new queue*/

	if((queue_id=msgget(key,IPC_CREAT|0666))==-1) /*get the queue*/
		printf("Error al iniciar la cola\n");
	else{
		while(!end) {
			msgrcv(queue_id,&qbuffer,MAX_OPT,REQ,0);
			if(qbuffer.mtype != END) {
				/*depending on the type of the msg*/
				if(qbuffer.scope==ISPRIVATE){
					switch(qbuffer.cmd) {
						/*MOVE FILE*/
						case MOVE:
							if (!qbuffer.ud) { // if download
								strcat(org, qbuffer.user.user_folder);
								strcat(org, "/");
								strcat(org, qbuffer.file);
								strcat(dest, qbuffer.fold); // move to working directory
							} else { // if upload
								strcat(dest, qbuffer.user.user_folder);
								strcat(org, qbuffer.fold); // move from working directory
								strcat(org, "/");
								strcat(org, qbuffer.file);
							}
							res->error = move(org,dest);
					 	break;

					 	/*DELETE FILE*/
					 	case DELETE:
							res->error = dlt(qbuffer.user.user_folder, qbuffer.file);
					 	break;
					}

				}
					
				else if(qbuffer.scope==ISPUBLIC) {
					clavem=ftok("../res/share/.",'A');
					shmid = co_mm(clavem,TAM_MEMORY);
					mutex = co_sem("mutex", RC);
					/*map the memory*/
					if((file =shmat(shmid,NULL,0)) == (File *)-1)
						printf("Error al mapear la memoria compartida\n");
					else{
						if (!qbuffer.ud) { // if download
							strcat(org, DEST_PUBLIC_PATH);
							strcat(org, qbuffer.file);
							strcat(dest, qbuffer.fold); // move to working directory
						} else { // if upload
							strcat(dest, DEST_PUBLIC_PATH);
							strcat(org, qbuffer.fold); // move from working directory
							strcat(org, "/");
							strcat(org, qbuffer.file);
						}
						sem_wait(mutex);
						free = used(qbuffer.file, file);
						sem_post(mutex);
						if(free) {
							switch(qbuffer.cmd) {
								/*MOVE FILE*/
								case MOVE:
									res->error = move(org,dest);
									break;
								/*DELETE FILE*/
								case DELETE:
									res->error = dlt(DEST_PUBLIC_PATH, qbuffer.file);
									break;
							}
						sem_wait(mutex);
						nused(qbuffer.file, file);
						sem_post(mutex);
						}
					}
				}
				res->mtype = RES;
				msgsnd(queue_id, res, MAX_RES, 0);
			}
			else
				end = 1;
		}
	}
}



