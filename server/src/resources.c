#include "server.h"


#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h> 
#include <string.h> 
#include <ctype.h> 

#define ERROR_OSEM 1
#define RC 1
#define ERROR_MM 2
#define ERROR_COLA 3
#define ERROR_CSEM 4


int init(int users){

	/* Creacion de dos semaforos. El semaforo llamado users_on gestiona el numero de usuarios 
		que pueden estar conectados a la vez en su carpeta privada. El semaforo mutex gestiona
		el usuario que puede estar accediendo a la carpeta compartida.*/

	sem_t *users_on, *mutex=NULL;
	int error = 0;

	users_on =sem_open("users", O_CREAT,0600,users);
	mutex = sem_open("mutex", O_CREAT,0600,RC);
	if(users_on == NULL || mutex == NULL) {
		error = ERROR_OSEM;
	}
	else{
		sem_close(users_on);
		sem_close(mutex);
	}


	/*Creacion de la memoria compartida para la comunicación entre procesos*/

	key_t clavem; 

   
   	clavem=ftok("memory",'A');
   	
   	if((shmget(clave,TAM_MEMORY,IPC_CREAT|IPC_EXCL|0660))==-1) {
   		error  = ERROR_MM;
   	}


   	/*Creacion de cola de mensajes para comunicacion entre procesos*/



   	key_t clavec;

   	key_t clavec = ftok("cola", 'P');

   	if(msgget(clave,IPC_CREAT|0660) ==-1){
   		error = ERROR_COLA;
   	}
   	
return(error);

}


int end(){
	
	int error;
	int error1 = sem_unlink("users_on");
	int error2 = sem_unlink("mutex");

	if (error1 != 0 || error2 != 0){
		error = ERROR_CSEM;
	}

	





}