/***INCLUDES****/
#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h> 
#include <sys/stat.h>/* struct stat y fstat */
#include <sys/mman.h>

/***APPLICATION FILES**/
#include "./log_controller"


/***DEFINES***/
#define MAX_SEND_SIZE 80
#define MAX_FILENAME  50
#define MAP_TYPE 1
#define ACTION 2
#define CD 3     /*change directory*/
#define ISPRIVATE "private"
#define ISPUBLIC "public"



struct mymsgbuf{ 
   long mtype;
   char mtext[MAX_SEND_SIZE];
   char filename[MAX_FILENAME]; 
};

void *th_controller(void * con){

key_t key;
int queue_id;
long tipo;
Conn * conn = (Conn *) con;
char * user_name = conn->user.user_name;

struct mymsgbuf qbuffer;
key=ftok(user_name,'P');

if((queue_id=msgget(key,IPC_CREAT|0666))==-1)
	printf("Error al iniciar la cola\n");
else{
	if(msgrcv()!=-1){
		tipo=qbuffer.mtype;
		switch(tipo){
			case MAP_TYPE:	
				map_type();
			break
			case ACTION:

				/*Waiting the functions be done*/

			break
			case CD:/*change directory*/
				cd();
			break
			default:
				printf("Tail type incorrect\n");
			break
		}
	}
}

}


void map_type(){

int struct stat bstat;
char * mapped_file;
int fd; //File descriptor

if((fd=open(qbuffer.filename,O_RONLY))<0) //First open the file
	printf("No pudo abrirse el fichero\n");
	
else if(fstat(fd,&bstat)){ //get file status
	printf("Error en el fstat del fichero\n");
}
else if(ftruncate(fd,bstat.st_size)<0) //get the size of the file
	printf("Error en el ftruncate en el fichero\n");

/*Then, map the file in to the process*/
if(qbuffer.text == ISPRIVATE){ //privated mapping
	if((mapped_file=(char * )mmap((caddr_t)0,bstat.st_size,PROT_WRITE,MAP_PRIVATE,fd,0))==MAP_FAILDED)
		printf("Error en la proyeccion de fichero\n");

	//Now what??

else{  //otherwise mapping is public 
	if((mapped_file=(char * )mmap((caddr_t)0,bstat.st_size,PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILDED)
		printf("Error en la proyeccion de fichero\n");

	//Now What???
}

}
}

