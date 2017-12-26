#include <sys/stat.h> 
#include <sys/mman.h> 
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

#define NO_ERROR 0
#define TAM_RT 100
#define ERROR -1
#define BARRA "/"




int delete(char *rt, char *name){

int error = NO_ERROR;

char *delete =strcat(rt,(const char*)name);
if (unlink((const char *)delete) == -1){
	error = ERROR;
} 
return error;

}