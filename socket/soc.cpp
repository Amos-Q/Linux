#include <cstdio.h>
#include <string>
#include <sys/socket.h>//套接字接口信息
#include <netinet/in.h>//包含地址信息
#include <arpa/inet.h>//字节序转换接口

class UdpSocket{
	public:
		bool Socket(){//创建套接字
			//socket(地址域，套接字类型，协议类型)
			_socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
			if(_socket < 0){
				perror("socket error");
				return false;
			}
			return true;
		}
		//为套接字绑定地址信息
		bool Bind(const std::string &ip, uint16_t port){
			//定义ipv4地址结构 struct sockaddr_in
			struct sockaddr_in addr;
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);//htons将主机字节序短整型数据转换为网络字节序数据
			addr.sin_addr.s_addr = inet_addr(ip.c_str());//将字符串IP地址转换为网络字节序
			//bind(描述符，地址信息，地址信息长度)
			socklen_t len = sizeof(struct sockaddr_in);
			int ret = bind(_sockfd,(struct sockaddr*)&addr,len);
			if(ret < 0){
				perror("bind error");
				return false;
			}
			return true;
		}
		//接受数据，获取发送端地址信息
		bool Recv(std::string *buf,std::string *ip,const uint16_t port)
			//发送数据
			bool Send(const std::string &data,const std::string &ip,const uint16_t port)
			bool Close();//关闭套接字
	private:
		int _sockfd;
};
