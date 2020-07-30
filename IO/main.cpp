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
                //就绪的描述符与监听套接字描述符一样，就表示需要获取新连接
                TcpSocket new_sock;
                ret = lst_sock.Accept(&new_sock);
                if (ret == false) {
                    continue;
                }
                s.Add(new_sock);//将新建套接字也添加监控
            }else {
                //就绪的描述符不是监听套接字，那么就是通信套接字，则进行recv
                std::string buf;
                ret = sock.Recv(&buf);
                if (ret == false) {
                    sock.Close();
                    s.Del(sock);//关闭套接字则需要移除监控
                    continue;
                }
                std::cout << "client say: " << buf << std::endl;
                std::cout << "server say: ";
                buf.clear();
                std::cin >> buf;
                ret = sock.Send(buf);
                if (ret == false) {
                    sock.Close();
                    s.Del(sock);//关闭套接字则需要移除监控
                }
            }
        }
    }
    lst_sock.Close();

    return 0;
}