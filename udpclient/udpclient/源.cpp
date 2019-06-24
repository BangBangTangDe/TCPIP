#include<iostream>
#include<assert.h>
#include<string>
#include<stdio.h>
#include<WinSock2.h>
#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")
using namespace std;
const int udp_port = 10001;
bool init()
{
	WORD versionrequisted;
	WSADATA data;
	int err;
	versionrequisted = MAKEWORD(2, 2);
	err = WSAStartup(versionrequisted, &data);
	if (err != 0)
	{
		return false;

	}
	if (LOBYTE(data.wVersion) != 2 || HIBYTE(data.wVersion != 2))
	{
		WSACleanup();
		return false;
	}
	return true;

}
void finit()
{
	WSACleanup();
}
SOCKET Creatudpcast()
{

	SOCKET sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == sock)
	{
		cout << "创建套接字失败" << endl;
		return INVALID_SOCKET;
	}
	bool flag = true;
	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&flag, sizeof(flag));
#ifndef SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)
#endif // SIO_UDP_CONNRESET
	bool behavior = false;
	DWORD dwbytereturn = 0;
	WSAIoctl(sock, SIO_UDP_CONNRESET, &behavior, sizeof behavior, NULL, 0, &dwbytereturn, NULL, NULL);
	return sock;
}
bool echo()
{
	SOCKET sock = Creatudpcast();
	assert(INVALID_SOCKET != sock);
	SOCKADDR_IN addrto = { 0 };
	addrto.sin_family = AF_INET;
	addrto.sin_port = htons(udp_port);
	addrto.sin_addr.s_addr = INADDR_BROADCAST;
	int len = sizeof(SOCKADDR);
	
	const char * sendbuf  = "客户端的测试";
	int sendlen = strlen(sendbuf);
	int send = sendto(sock, sendbuf, sendlen, 0, (SOCKADDR*)&addrto, len);
	if (send <= 0)
	{
		closesocket(sock);
		cout << "发送错误" << endl;
		return false;
	}
	char recv[1024] = { 0 };
	int nrecv = recvfrom(sock,recv,1024,0,(SOCKADDR*)&addrto,&len);
	if (nrecv <= 0)
	{
		closesocket(sock);
		cout << "接受失败" << endl;
		return false;
	}
	string ip(inet_ntoa(addrto.sin_addr));
	printf("接收的信息是%s, ip: port=%s:%d\n", recv, ip.c_str(), addrto.sin_port);
	return true;
}
int main()
{
	init();
	echo();
	finit();
	getchar();
	return 0;
}