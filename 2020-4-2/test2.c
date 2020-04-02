#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	//close(1);
	umask(0);
	int fd= open("./test.txt",O_RDWR|O_CREAT,0664);
	if(fd<0){
		perror("open error");
		return -1;
	}
	//dup2(fd,1);
	printf("fd=%d\n",fd);
	fflush(stdout);
	close(fd);
}
