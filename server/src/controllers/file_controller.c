#include <sys/stat.h> 
#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#define NO_ERROR 0
#define TAM_RT 100
#define FAIL -1
#define ERROR_OPEN0 1
#define ERROR_OPEND 4
#define ERROR_FSTAT 2
#define ERROR_CRT 3
#define ERROR_CLOSE 5



/*Delete name in rt*/
int dlt(char *rt, char *name){


int tam = strlen(rt) + strlen(name);
char aux[tam];
int error = NO_ERROR;
strcpy(aux,rt);
strcat(aux,"/");
strcat(aux,name);
if (unlink((const char *)aux) == -1){
	error = FAIL;
} 
return error;

}

/*Copy a file to another directory. Return error*/

int move(char* orig, char* dest){

	int ficho, fichd;
	int error = NO_ERROR;
	struct stat aux;
	char *o, *d, *ds,*or;

	/*Open the file, which we want to move*/
	if((ficho = open(orig,O_RDONLY)) == -1){
		error = ERROR_OPEN0;
	}
	/*Open the directory where we want to copy the file*/
	else if((fichd = open(dest,O_CREAT|O_TRUNC|O_RDWR,0640)) == -1){
		error = ERROR_OPEND;
		close(ficho);
	}

	/*Take the size of the file to create the new file*/
	else if ((fstat(ficho,&aux))){
		error = ERROR_FSTAT;
		close(ficho);
		close(fichd);
	}

	/*Set the size of the new file*/
	else if (ftruncate(fichd,aux.st_size) == -1)
	{
		error = ERROR_CRT;
		close(ficho);
		close(fichd);
	}
	else{
		/*Mapping the file and the new file in the memory to move then*/
		or = (char *)mmap((caddr_t) 0,aux.st_size, PROT_READ , MAP_SHARED ,ficho ,0);
		ds = (char *)mmap((caddr_t) 0,aux.st_size, PROT_WRITE,MAP_SHARED,fichd,0);
		close(fichd);
		close(ficho);
		if(or==MAP_FAILED || ds == MAP_FAILED){
			error=ERROR_CRT;
		}
		else{
			int i;
			o=or;
			d=ds;
			/*Loop for copy the file in his new directory*/
			for (i = 0; i < aux.st_size ; i++){
				*d++ = *o++;
				
			}
		/*Unmapping the memory*/
		if(munmap(or,aux.st_size) == -1){
			error = ERROR_CLOSE;
			unlink(dest);
		}
		if (munmap(ds,aux.st_size) == -1){
			error = ERROR_CLOSE;
			unlink(dest);
		}

		}
	}

	return error;
}