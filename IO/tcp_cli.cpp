#include <iostream>
#include "tcpsocket.hpp"

int main (int argc, char *argv[])
{
    if (argc != 3) {
        std::cout << "Usage: ./tcp_cli ip port\n";
        return -1;
    }
    std::string srv_ip = argv[1];
    uint16_t srv_port = std::stoi(argv[2]);

    TcpSocket sock;
    CHECK_RET(sock.Socket());
    CHECK_RET(sock.Connect(srv_ip, srv_port));
    while(1) {
        std::string buf;
        std::cout << "client say: ";
        std::cin >> buf;
        sock.Send(buf);

        buf.clear();
        sock.Recv(&buf);
        std::cout << "server say: " << buf << std::endl;
    }
    sock.Close();
    return 0;
}
