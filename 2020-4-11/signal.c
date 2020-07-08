#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sigcb(int signo)
{
	printf("recv a signal no:%d\n",signo);
}


int main()
{
	signal(SIGINT,sigcb);
	signal(3,sigcb);
	while(1){
		printf("HELLO\n");
		sleep(1);
	}
	return 0;
}
