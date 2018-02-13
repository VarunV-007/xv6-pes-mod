#include "types.h"
#include "user.h"
#include "login_credentials.h"

static char buf[100];
int nbuf=sizeof(buf), t;

void user_wait() {
	printf(1,"Wait for some time to try again!\n\n");
	sleep(wait_time);
	t = attempts;
}
void user_input() {
	memset(buf, 0, nbuf);
  	gets(buf, nbuf);
	buf[strlen(buf)-1]='\0'; //Replaces '\n' with '\0'
}
int verify() {
	t = attempts; 
	while(1) {	//Takes username
		if (t==0) user_wait();
		printf(1,">> Enter Username : ");
		user_input();
		if (strcmp(buf,username) == 0) break;
		printf(1,"\nUser %s does not exist. (%d attempt(s) remaining)\n\n",buf,--t);
	}
	t = attempts;
  	while(1) {	//Takes password	
		if (t==0) user_wait();
		printf(1, ">> Enter Password : ");
  		user_input();
		if (strcmp(buf,password) == 0) break;
		printf(1,"\nWrong Password! (%d attempt(s) remaining)\n\n",--t);
	}
	printf(1,"\nWelcome %s!\n\n",username);
	return 1;
}
