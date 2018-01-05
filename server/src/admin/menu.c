/*
** Server menu
*/

/*APPLICATION  FILES*/
#include "../server.h"


/*INCLUDES*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h> 

void menu(){
	char  order; //order represented by numeric character.
	pid_t id;
	User user;
	
	
	/*Waiting orders*/
	while(true){
		printf("What do you want to do at the server menu?\n");
		printf("1: Create user\n");
		printf("2: Delete user\n");
		printf("3: Update\n");
		printf("4: Exit\n");
		printf("Select one: ");
		scanf("%s",&order);

		switch (order){
			case '1':
				printf("Introduce username and password\n");
				printf("Username: ");
				scanf("%s",user.user_name);
				printf("password: ");
				scanf("%s",user.password);
				/*Create user function*/
				if(create_user(user)) //it returns OK if success. 
					printf("Success!\n");
				else
					printf("Error creating user\n"); // it returns ERROR otherwise.
				break;

			case '2':
				printf("Introduce username in order to delete it:\n");
				printf("Username: ");
				scanf("%s",user.user_name);
				/*delete user function*/
				if(delete_user(user))
					printf("Success!\n");
				else
					printf("Error deleting user\n");
				break;

			case '3':
				printf("Introduce username and new password\n");
				printf("Username: ");
				scanf("%s",user.user_name);
				printf("password: ");
				scanf("%s",user.password);
				/*update user function*/
				if(update_user(user))
					printf("Success!\n");
				else
					printf("Error updating user\n");
				break;

			case '4':
				/*Send SIGKILL signal to exit the menu*/
				id= getpid();
				kill(id,SIGKILL); //send signal to this process.
				break;
			default: 
				printf("%s it is not an order",&order);
				break;
		}
	}
}
