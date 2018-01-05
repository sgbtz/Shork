#include "../client.h"


#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include "../controllers/res_controller.c"


#define PRIV 2
#define SHARE 1
#define DOWNL 0
#define UPL 1
#define END -1

int showmenu(){

		int action;
		printf("Now introduce what do you want to do:\n");
		printf("1. SEE THE FILES\n");
		printf("2. DOWNLOAD A FILE\n");
		printf("3. UPLOAD A FILE\n");
		printf("4. DELETE A FILE\n");
		printf("5. CHANGE THE SCOPE\n");
		printf("6. EXIT\n");
		printf("Introduce a number: ");
		scanf("%d", &action);

		return action;
}


void menu(int msgid, User *user){

	unsigned scope = PRIV;
	int cont = 1;
	int action;
	int sscop = 1;
	Opt * end = malloc(MAX_OPT_SIZE);

	while (cont){
		printf("Welcome. What folder do you want to open? Enter the number:\n");
		printf("1. SHARE\n");
		printf("2. PRIVATE\n");
		printf("Introduce a number: ");
		scanf("%u", &scope);
		sscop = 1;
		
		while(sscop){

			action = showmenu();
			switch(action){
				case 1:
					show(scope,user);
					break;
				case 2:
					move(msgid,scope,user,DOWNL);
					break;
				case 3:
					move(msgid,scope,user,UPL);
					break;
				case 4:
					delt(msgid,scope,user);	
					break;
				case 5:
					sscop=0;
					break;
				case 6:
					sscop = 0;
					cont = 0;
					break;
				}

			}
		}
	
	end->mtype = END;
	msgsnd(msgid, end, MAX_OPT_SIZE, 0);
	printf("Thank you for use our service. See you soon.\n");
	msgctl(msgid,IPC_RMID, NULL);
}

