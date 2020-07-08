#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int a =1;
int b =1;

int test(){
	a++;
	sleep(3);
	b++;
	return a+b;
}

void sigcb(int no)
{
	printf("signal sum:%d\n",test());
}

int main()
{
	signal(SIGINT,sigcb);
	printf("main sum:%d\n",test());
	return 0;
}
