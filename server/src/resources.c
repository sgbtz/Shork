#include "./server.h"
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h> 
#include <string.h> 
#include <ctype.h> 
#include <sys/msg.h> 
#include <dirent.h>
#include <sys/types.h>


#define ERROR_OSEM 1
#define RC 1
#define ERROR_MM 2
#define ERROR_COLA 3
#define ERROR_CSEM 1
#define ERROR_DELMM 2
#define ERROR_DELC 3



/*Map the share memory with all the files in the route since the variable i. Return the share memory mapped*/

File *map_folder(char* route, int shmid, int * i){
	File *file = NULL;
	file = shmat(shmid,NULL,0);
	DIR *actual = opendir(route);
	if(actual != NULL){
		struct dirent *aux;
		while((aux = readdir(actual)) != NULL){
			strncpy(file[*i].name,aux->d_name,MAX_NAME_FOLD-1);
			file[*i].status = 1;
			(*i)++;
		}
	}
	else{
		printf("The route doesn't exist.\n");
	}
	return file;	
}

/*Search if the file with the same name that "name" is free*/
int used(char *name, File *file){
	int status = 0;
	int i = 0;
	int aux = 1;

	while(i < MAX_FILES && aux){
		if(!strncmp(file[i].name,name,strlen(name))){
			if ((status = file[i].status))
				file[i].status = 0;

			aux = 0;
		}
		i++;
	}
	return status;
}
/* Set as free the file status */
void nused(char * name, File * file) {
	int i = 0;
	int aux = 1;

	while(i < MAX_FILES && aux){
		if(!strncmp(file[i].name,name,strlen(name))){
			file[i].status = 1;
			aux = 0;
		}
		i++;
	}
}

/*This function create or open a semaphore.Recive the name of the semaphore
  and return the id */ 

sem_t *co_sem(char* name, int tam){

	sem_t *sem = NULL;
	sem = sem_open(name, O_CREAT,0600,tam);
	return(sem);
}

/* This function create or open a share memory. Recive the key and the long of the 
	part will be create. Finally return the id of the share memory.*/

int co_mm(key_t clave, int tam){

	int shmid = shmget(clave,tam,IPC_CREAT|0660);

	return(shmid);
}

/* This function create or open a tail. Recive the key and return the id */

int co_cola(key_t clave){

	int colaid = msgget(clave,IPC_CREAT|0660);
	return(colaid);
}

/*This function delete a share memory which was create.Recive the share memory's key and return if an error happens.*/

int dt_mm(int shmid){

	int error = 0;

	if(shmctl(shmid,IPC_RMID,NULL) == -1){	
 		error = ERROR_DELMM;
    }

    return(error); 
}

/*This function delete a tail which was create. Recive the tail's key and return if an error happens*/

int dt_c(int msgid){

	int error = 0;
	
	
	if ( msgctl(msgid,IPC_RMID, NULL) == -1){
		error = ERROR_DELC;
	}
	return(error);
}


/*This function initialize a programme which need semaphore, share memory and tails. */

int init(){

	/* Create a semaphore. This semaphore have the number of users who can entry in the share memory*/

	sem_t *mutex=NULL;
	int error = 0;
	int shmid;
	int * nfiles = malloc(sizeof(int));
	*nfiles = 0;

	mutex = co_sem("mutex", RC);
	if(mutex == NULL) {
		error = ERROR_OSEM;
	}
	else{
		sem_close(mutex);
	}



	/*Creation of the share memory*/

	key_t clavem=ftok("server/res/share/.",'A'); 


   	
   	if((shmid = co_mm(clavem,TAM_MEMORY))==-1) {
   		error  = ERROR_MM;
   	}

   	map_folder("server/res/share",shmid,nfiles);


   	/*Creation of the tail*/

   	key_t clavec = ftok("server/res/share/.", 'P');

   	if(co_cola(clavec) == -1){
   		error = ERROR_COLA;
   	}


   	
return(error);

}


/*THis function delete all the resources which was used by the programme*/


int end(){
	
	int error = 0;
	int error2 = sem_unlink("mutex");

	if (error2 != 0){
		error = ERROR_CSEM;
	}

	
	key_t clavem = ftok("server/res/share/.",'A');
	int shmid = co_mm(clavem,TAM_MEMORY);
	error = dt_mm(shmid);

	key_t clavec = ftok("server/res/share/.",'P');
	int msgid = co_cola(clavec);
	error = dt_c(msgid);

	return(error);

}