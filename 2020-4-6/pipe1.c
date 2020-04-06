#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	//匿名管道只能用于具有亲缘关系的进程之间
	//注意创建管道一定要放在创建子进程之前
	int pipefd[2]={-1};//pipefd[0]-读数据 pipefd[1]-写数据
	int ret =pipe(pipefd);
	if(ret <0){
		perror("pipe error");
		return -1;
	}
	pid_t pid = fork();
	if (pid == 0){
		//子进程从管道读数据
		char buf[1024]={0};
		read(pipefd[0],buf,1023);
		printf("child:%s",buf);
	}else if(pid > 0){
		//父进程向管道写入数据
		char *ptr = "HEELO\n";
		int total_len=0;
		while(1){
			int wlen=write(pipefd[1],ptr,strlen(ptr));
			total_len += wlen;
			printf("total_len=%d\n",total_len);
		}
	}
	return 0;
}
