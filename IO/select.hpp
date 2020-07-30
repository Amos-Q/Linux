#include <cstdio>
#include <vector>
#include <sys/select.h>
#include "tcpsocket.hpp"

#define MAX_TIMEOUT 3000
class Select{
    public:
        Select():_maxfd(-1){
            FD_ZERO(&_rfds);//��ʼ����ռ���
        }//��ʼ������
        bool Add(TcpSocket &sock) {
            int fd = sock.GetFd();
            FD_SET(fd, &_rfds);
            _maxfd = _maxfd > fd ? _maxfd : fd;//ÿ���޸ļ��϶�Ҫ�����ж����������
            return true;
        }//��sock�е���������ӵ����漯����
        bool Del(TcpSocket &sock) {
            int fd = sock.GetFd();
            FD_CLR(fd, &_rfds);
            for (int i = _maxfd; i >= 0; i--) {
                if (FD_ISSET(i, &_rfds)) {//�Ƴ�֮�󡣴Ӻ���ǰ�жϵ�һ�����ڼ����еľ�������
                    _maxfd = i;
                    break;
                }
            }
            return true;
        }//�ӱ��漯�����Ƴ���������������ټ�����������
        bool Wait(std::vector<TcpSocket> *list, int outtime = MAX_TIMEOUT) {
            struct timeval tv;
            tv.tv_sec = outtime / 1000;//outtime�Ժ���Ϊ��λ
            tv.tv_usec = (outtime % 1000) * 1000; //����ʣ���΢��
            fd_set set;
            set = _rfds;//����ֱ��ʹ��_rfds,��Ϊһ��_rfds���޸ľ���Ҫ�ⲿ���������������
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
                    //���ڼ�����֤���Ǿ����ģ���Ϊû�ж��Ᵽ�������������ֻ�ܴ�0��ʼ�ж�
                    TcpSocket sock;
                    sock.SetFd(i);
                    list->push_back(sock);
                }
            }
            return true;
        }//���м�أ�����ֱ�������ṩ������TcpSocket
    private:
        //����Ҫ��ؿɶ��¼���������---ÿ�μ��ʹ�õļ��϶���������ϵĸ��ư棨select���޸ļ��ϣ�
        fd_set _rfds; 
        //ÿ�μ�ض���Ҫ��������������
        int _maxfd;
};