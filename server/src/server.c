#include "server.h"
#include "controllers/user_controller.c"

#include <stdio.h>
#include <stdlib.h>

int main() {
	User * user = NULL;

	user = malloc(sizeof(char)*(MAX_PASS*MAX_UNAME*MAX_FOLD_URL + 3));
	printf("Usuario: ");
	scanf("%s",user->user_name);
	printf("Contraseña: ");
	scanf("%s",user->password);
	auth_user(user);
	return 0;
}