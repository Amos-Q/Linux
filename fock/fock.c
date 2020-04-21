#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	printf("-----creat child process start-----%d\n",getpid());
	int pid;
	pid = fork();
	if(pid == 0){
		printf("this is child %d--rpid:%d\n",getpid(),pid);
		sleep(1);
		exit(0);
	}
	else if(pid >0){
		printf("this is parent %d--rpid:%d\n",getpid(),pid);
	}
	return 0;
}
