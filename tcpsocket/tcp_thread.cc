#include <iostream>
#include <pthread.h>
#include "tcpsocket.hpp"

void *thr_worker(void *arg)
{
	long fd = (long)arg;//这种强转会被警告：大空间转小空间会涉及到数据截断
	TcpSocket new_sock;
	new_sock.SetFd(fd);
	while(1) {
		std::string buf;
		new_sock.Recv(&buf);//通过新建连接与指定客户端进行通信
		std::cout << "client say: " << buf << std::endl;
		buf.clear();
		std::cout << "server say: ";
		std::cin >> buf;
		new_sock.Send(buf);
	}
	new_sock.Close();//线程之间文件描述符表共享，这边关闭了描述符其它地方就都用不了了
	return NULL;
}
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
		//TcpSocket *new_sock = new TcpSocket();
		bool ret = lst_sock.Accept(&new_sock);//通过监听套接字获取新建连接
		if (ret == false) {
			continue;//服务端不能因为或一个新建套接字失败就退出
		}
		pthread_t tid;
		//new_sock是一个局部对象，循环完毕的时候就会被释放,传地址会造成错误
		//除非每次accept的时候new_sock在堆上申请一个新的，不会自动释放，也要防止第二次获取时候
		//覆盖上次的值
		//或者直接传值在函数栈中会新建空间存储值与原数据空间就没关系了-仅限于数据占用空间比较小
		int res = pthread_create(&tid, NULL, thr_worker, (void*)new_sock.GetFd());
		if (res != 0) {
			std::cout << "pthread create error\n";
			continue;
		}
		pthread_detach(tid);//不关心线程返回值，也不想等待释放资源，因此将线程分离出去
		//new_sock.Close();//主线程不能关闭描述符，因为线程间描述符表是共享的
	}
	lst_sock.Close();
	return 0;
}

