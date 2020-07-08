#include <iostream>
#include "tcpsocket.hpp"

int main (int argc, char *argv[])
{
	if (argc != 3) {
		std::cout << "Usage: ./tcp_srv ip port\n";
		return -1;
	}
	std::string ip = argv[1];
	uint16_t port = std::stoi(argv[2]);

	TcpSocket lst_sock;
	CHECK_RET(lst_sock.Socket());//创建监听套接字
	CHECK_RET(lst_sock.Bind(ip, port));//为监听套接字绑定地址
	CHECK_RET(lst_sock.Listen());//开始监听
	while(1) {
		TcpSocket new_sock;
		bool ret = lst_sock.Accept(&new_sock);//通过监听套接字获取新建连接
		if (ret == false) {
			continue;//服务端不能因为或一个新建套接字失败就退出
		}
		int fd = new_sock.GetFd();
		cal_t cal;
		//定义申请了一块9个字节的空间，这块空间的数据类型是cal_t
		//recv第二个参数是一块空间的首地址，这个接口要求char*
		recv(fd, (char*)&cal, sizeof(cal_t), 0);
		printf("%d %c %d\n", cal.num1, cal.op, cal.num2);
	}
	lst_sock.Close();
	return 0;
}
