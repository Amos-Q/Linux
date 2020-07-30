#include <cstdio>
#include <vector>
#include <unistd.h>
#include <sys/epoll.h>
#include "tcpsocket.hpp"

class Epoll{
	public:
		Epoll() {
			//epoll_create(size)；  size已经被忽略，大于0即可
				_epfd = epoll_create(1);
			if (_epfd < 0) {
				perror("epoll create error");
				_exit(0);
			}
		}
		bool Add(TcpSocket &sock) {
			int fd = sock.GetFd();
			//定义一个描述符对应的事件结构体 struct epoll_event；
			struct epoll_event ev;
			ev.data.fd = fd;//fd描述符就绪后，返回的这个结构中的data.fd就是我们要操作的描述符
			ev.events = EPOLLIN;//表示监控可读事件-EPOLLIN; EPOLLOUT-可写事件 EPOLLIN | EPOLLOUT
			//epoll_ctl(epoll句柄， 操作类型， 监控的描述符， 描述符对应事件结构)
			int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev);
			if (ret < 0) {
				perror("epoll_ctl_add error");
				return false;
			}
			return true;
		}
		bool Del(TcpSocket &sock) {
			int fd = sock.GetFd();
			int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
			if (ret < 0) {
				perror("epoll_ctl_del error");
				return false;
			}
			return true;
		}
		bool Wait(std::vector<TcpSocket> *list, int timeout = 3000) {
			//epoll_wait(句柄， 接收就绪结构的数组， 数组节点数量， 超时事件);
			//返回值大于0表示就绪的个数
			struct epoll_event evs[10];
			int ret = epoll_wait(_epfd, evs, 10, timeout);
			if (ret < 0) {
				perror("epoll_wait error");
				return false;
			}else if (ret == 0) {
				printf("epoll timeout\n");
				return false;
			}
			for (int i = 0; i < ret; i++) {
				if (evs[i].events & EPOLLIN) {//判断是否就绪的可读事件
					TcpSocket sock;
					sock.SetFd(evs[i].data.fd);
					list->push_back(sock);
				}
				if (evs[i].events & EPOLLOUT) {//不能使用等于号，因为有可能就绪了多个事件
					//......
				}
			}
			return true;
		}
	private:
		int _epfd;
};

