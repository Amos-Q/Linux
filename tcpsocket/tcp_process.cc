#include <iostream>
#include <signal.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

void sigcb(int no) {
	//SIGCHLD信号是一个非可靠信号，有可能丢失
	//因此在一次信号处理中，就需要处理到没有子进程退出为止
	while(waitpid(-1, NULL, WNOHANG) > 0);//返回值大于0，表示有子进程退出
}
int main (int argc, char *argv[])
{
	if (argc != 3) {
		std::cout << "Usage: ./tcp_srv ip port\n";
		return -1;
	}
	signal(SIGCHLD, sigcb);
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
		int pid = fork();//子进程复制父进程，父进程有的子进程都有
		if (pid == 0) {
			while(1) {
				std::string buf;
				new_sock.Recv(&buf);//通过新建连接与指定客户端进行通信
				std::cout << "client say: " << buf << std::endl;

				buf.clear();
				std::cout << "server say: ";
				std::cin >> buf;
				new_sock.Send(buf);
			}
			new_sock.Close();//new_sock父子进程各有各的  ,父子进程数据独有
			exit(0);
		}
		new_sock.Close();//父进程关闭自己的不使用的socket，不影响子进程
	}
	lst_sock.Close();
	return 0;
}
