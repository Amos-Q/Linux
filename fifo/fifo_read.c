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
	int fd = open("./test2.fifo",O_RDONLY);
	if(fd < 0){
		perror("open error");
		return -1;
	}
	printf("open fifo success\n");
	while(1){
		char buf[1024] = {0};
		int ret = read (fd,buf,1023);
		if(ret <0){
			perror("read error");
			return -1;
		}else if(ret == 0){
			perror("all write closed");
			return -1;	
		}
		printf("read buf:[%s]\n",buf);
	}
	close(fd);
	return 0;
}

