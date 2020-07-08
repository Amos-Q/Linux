#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
	//1.创建套接字
	int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd < 0) {
		perror("socket error");
		return -1;
	}
	//2.绑定地址信息（不推荐）
	//3.发送数据: 要给谁发送什么数据--发送的对端地址一定是服务端绑定的地址
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(8000);
	srv_addr.sin_addr.s_addr = inet_addr("192.168.245.3");
	socklen_t len = sizeof(struct sockaddr_in);
	char tmp[1024] = {0};
	fgets(tmp, 1024, stdin);
	sendto(sockfd, tmp, strlen(tmp), 0, (struct sockaddr*)&srv_addr, len);
	//4.接收数据
	char buf[1024] = {0};
	//对于客户端来说，本身久知道服务端地址，因此其实根本不用接收服务端地址信息
	recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*)&srv_addr, &len);
	printf("server say:%s\n", buf);
	//5.关闭套接字
	close(sockfd);
	return 0;
}

