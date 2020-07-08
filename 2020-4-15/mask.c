#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigcb(int signo)
{
	printf("recv a signal:%d\n",signo);
}

int main()
{
	signal(SIGINT,sigcb);
	signal(SIGRTMIN+4,sigcb);
	sigset_t set;
	sigemptyset(&set);//清空集合，防止未知数据造成影响
	sigfillset(&set);//向集合中添加所有信号
	sigprocmask(SIG_BLOCK,&set,NULL);
	printf("press enter coninue\n");
	getchar();
	sigprocmask(SIG_UNBLOCK,&set,NULL);//解除set集合中的信号阻塞
	while(1)
	{
		sleep(1);
	}
	return 0;
}
