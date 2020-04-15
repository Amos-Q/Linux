#include <stdio.h>
#include <unistd.h>
#include <signal.h>

long a = 1;

void sigcb(int no)
{
	a = 0;
	printf("a = %d\n",a);
}

int main()
{
	signal(SIGINT,sigcb);
	while(a){
	}
	printf("eixt a = %d\n",a);
	return 0;
}
