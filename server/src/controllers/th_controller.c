/***INCLUDES****/
#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

/***APPLICATION FILES**/
//#include "./log_controller"
#include "./file_controller.c"


/***DEFINES***/
#define MAX_SEND_SIZE 20
#define MAX_FILENAME  50
#define MAP_TIPO 1
#define ACTION 2
#define CD 3     /*change directory*/
#define ISPRIVATE "private"
#define ISPUBLIC "public"
#define MOVE "move"
#define DEL "delete"
#define DEST_PATH "../res/shared/"

void map_type(char * filename,char * perm);

struct mymsgbuf{ 
   long mtype;
   char action[MAX_SEND_SIZE];  /*Especificate what action want to do if type message is ACTION*/
   char content[MAX_SEND_SIZE]; /*value depends on what function you want to do*/
   char filename[MAX_SEND_SIZE];
};

void th_controller(){

key_t key;
int queue_id;
long tipo;
struct mymsgbuf qbuffer;/*Queue buffer*/

key=ftok("../res/share/.",'Q'); /*Creating new queue*/

if((queue_id=msgget(key,IPC_CREAT|0666))==-1) /*get the queue*/
	printf("Error al iniciar la cola\n");
else{
	if(msgrcv(queue_id,&qbuffer,MAX_SEND_SIZE,0,0)!=-1){
		tipo=qbuffer.mtype;
		/*depending on the type of the msg*/
		switch(tipo){ 
			/*MAP THE FILE IN THE PROCESS*/
			case MAP_TYPE: 
				map_type(qbuffer.filename,qbuffer.content); /*"private" or "public" as parameter*/ 
			break;
			
			case ACTION:
				/*MOVE FILE*/
				if(strcmp(qbuffer.action,MOVE)==0) /*add file or move file case*/
					move(qbuffer.content,DEST_PATH);
				/*DELETE FILE*/
				if(strcmp(qbuffer.action,DEL)==0)/*delete file case*/
					dlt(DEST_PATH,qbuffer.content);
			break;
			
			//case CD:/*change directory*/

			//break
			default:
				printf("Tail type incorrect\n");
			break;
		}
	}
}

}


void map_type(char * filename,char * perm){

struct stat bstat;
char * mapped_file;
int fd; //File descriptor

if((fd=open(filename,O_RDONLY))<0) //First open the file
	printf("No pudo abrirse el fichero\n");
	
else if(fstat(fd,&bstat)){ //get file status
	printf("Error en el fstat del fichero\n");
}
else if(ftruncate(fd,bstat.st_size)<0) //get the size of the file
	printf("Error en el ftruncate en el fichero\n");

/*Then, map the file in to the process*/
if(strcmp(perm,ISPRIVATE)==0){ //privated mapping
	if((mapped_file=(char * )mmap((caddr_t)0,bstat.st_size,PROT_WRITE,MAP_PRIVATE,fd,0))==MAP_FAILED){
		printf("Error en la proyeccion de fichero\n");
		printf("File has been mapped privated succesfuly\n");
	}
	//Now what??
	
//otherwise mapping is public
else{   
	if((mapped_file=(char * )mmap((caddr_t)0,bstat.st_size,PROT_WRITE,MAP_SHARED,fd,0))==MAP_FAILED){
		printf("Error en la proyeccion de fichero\n");
		printf("File has been mapped shared succesfuly\n");
	}
	//Now What???
	
}

}
}

