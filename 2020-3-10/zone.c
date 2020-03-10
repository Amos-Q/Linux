#include <stdio.h>
#include <unistd.h>

int gval=1;
int main()
{
	int pid = 0;
	pid =fork();
	if(pid == 0){
		gval=100;
		printf("child gval:%d---%p\n",gval,&gval);
	}
	else if(pid >0){
		sleep(3);
		printf("parent gavle:%d---%p\n",gval,&gval);
	}
	return 0;
}
