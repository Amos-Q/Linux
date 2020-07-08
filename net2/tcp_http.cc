#include <iostream>
#include "tcpsocket.hpp"
#include <sstream>
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
		std::string req;
		req.clear();
		if(new_sock.Recv(&req) == false) {
			new_sock.Close();
			continue;
		}
		std::cout<<"req:["<<req<<"]\n";
		std::string body = "<html><body><h1>Hello World</h1></body></html>";
		std::stringstream header;
		header << "Content-Length:"<<body.size()<<"\r\n";
		header << "Connection:close\r\n";
		header<<"Location:www.baidu.com\r\n";
		std::string blank = "\r\n";
		//std::string first_line = "HTTP/1.1 200 OK\r\n";
		std::string first_line = "HTTP/1.1 302 Found\r\n";
		new_sock.Send(first_line);
		new_sock.Send(header.str());
		new_sock.Send(blank);
		new_sock.Send(body);
		new_sock.Close();
	}
	lst_sock.Close();
	return 0;
}

