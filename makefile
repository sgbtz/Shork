#MAKEFILE OF SHORK

CC = gcc
OPT = -W -Wall 

shork: client.o server.o
	$(CC) $(OPT) -o client/bin/client client.o
	$(CC) $(OPT) -o server/bin/server server.o -lpthread

client.o: 
	$(CC) $(OPT) -c client/src/client.c

server.o: 
	$(CC) $(OPT) -c server/src/server.c 

delete:
	rm *.o