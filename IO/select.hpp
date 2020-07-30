#include <cstdio>
#include <vector>
#include <sys/select.h>
#include "tcpsocket.hpp"

#define MAX_TIMEOUT 3000
class Select{
    public:
        Select():_maxfd(-1){
            FD_ZERO(&_rfds);//初始化清空集合
        }//初始化操作
        bool Add(TcpSocket &sock) {
            int fd = sock.GetFd();
            FD_SET(fd, &_rfds);
            _maxfd = _maxfd > fd ? _maxfd : fd;//每次修改集合都要重新判断最大描述符
            return true;
        }//将sock中的描述符添加到保存集合中
        bool Del(TcpSocket &sock) {
            int fd = sock.GetFd();
            FD_CLR(fd, &_rfds);
            for (int i = _maxfd; i >= 0; i--) {
                if (FD_ISSET(i, &_rfds)) {//移除之后。从后往前判断第一个还在集合中的就是最大的
                    _maxfd = i;
                    break;
                }
            }
            return true;
        }//从保存集合中移除这个描述符，不再监控这个描述符
        bool Wait(std::vector<TcpSocket> *list, int outtime = MAX_TIMEOUT) {
            struct timeval tv;
            tv.tv_sec = outtime / 1000;//outtime以毫秒为单位
            tv.tv_usec = (outtime % 1000) * 1000; //计算剩余的微妙
            fd_set set;
            set = _rfds;//不能直接使用_rfds,因为一旦_rfds被修改就需要外部重新添加描述符了
            int ret = select(_maxfd+1, &set, NULL, NULL, &tv);
            if (ret < 0) {
                perror("select error");
                return false;
            }if (ret == 0) {
                printf("wait timeout\n");
                return true;
            }
            for (int i = 0; i < _maxfd+1; i++) {
                if (FD_ISSET(i, &set)) {
                    //还在集合中证明是就绪的，因为没有额外保存描述符，因此只能从0开始判断
                    TcpSocket sock;
                    sock.SetFd(i);
                    list->push_back(sock);
                }
            }
            return true;
        }//进行监控，并且直接向外提供就绪的TcpSocket
    private:
        //保存要监控可读事件的描述符---每次监控使用的集合都是这个集合的复制版（select会修改集合）
        fd_set _rfds; 
        //每次监控都需要输入的最大描述符
        int _maxfd;
};