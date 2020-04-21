#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
int main()
{
	umask(0);
	int ret = mkfifo("./test2.fifo",0664);
	if(ret < 0 && errno != EEXIST){
		perror("mkfifo error");
		return -1;
	}       
	int fd = open("./test2.fifo",O_WRONLY);
	if(fd < 0){
		perror("open error");
		return -1;
	}       
	printf("open fifo success\n");
	int i = 0;
	while(1){
		char buf[1024] = {0};
		sprintf(buf,"hello....+%d",i++);
		write(fd,buf,strlen(buf));
		sleep(1);
	}
	close(fd);
	return 0;
}

