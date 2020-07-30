#include <iostream>
#include "select.hpp"
#include "epoll.hpp"

int main (int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Usage: ./main ip port\n";
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = std::stoi(argv[2]);
    TcpSocket lst_sock;
    CHECK_RET(lst_sock.Socket());
    CHECK_RET(lst_sock.Bind(ip, port));
    CHECK_RET(lst_sock.Listen());

    //Select s;
	Epoll s;
    s.Add(lst_sock);
    while(1) {
        std::vector<TcpSocket> list;
        bool ret = s.Wait(&list);
        if (ret == false) {
            continue;
        }
        for (auto sock : list) {
            if (sock.GetFd() == lst_sock.GetFd()){
                //������������������׽���������һ�����ͱ�ʾ��Ҫ��ȡ������
                TcpSocket new_sock;
                ret = lst_sock.Accept(&new_sock);
                if (ret == false) {
                    continue;
                }
                s.Add(new_sock);//���½��׽���Ҳ��Ӽ��
            }else {
                //���������������Ǽ����׽��֣���ô����ͨ���׽��֣������recv
                std::string buf;
                ret = sock.Recv(&buf);
                if (ret == false) {
                    sock.Close();
                    s.Del(sock);//�ر��׽�������Ҫ�Ƴ����
                    continue;
                }
                std::cout << "client say: " << buf << std::endl;
                std::cout << "server say: ";
                buf.clear();
                std::cin >> buf;
                ret = sock.Send(buf);
                if (ret == false) {
                    sock.Close();
                    s.Del(sock);//�ر��׽�������Ҫ�Ƴ����
                }
            }
        }
    }
    lst_sock.Close();

    return 0;
}