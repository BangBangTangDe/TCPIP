#include<iostream>
#include<assert.h>
#include<string>
#include<stdio.h>
#include<WinSock2.h>
#pragma warning (disable:4996)
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
	if (err!=0)
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
bool run()
{

	SOCKET sock = Creatudpcast();
	assert(INVALID_SOCKET != sock);
	SOCKADDR_IN sin = { 0 };
	sin.sin_family = AF_INET;
	sin.sin_port = htons(udp_port);
	sin.sin_addr.s_addr = 0;
	int err = bind(sock, (SOCKADDR*)&sin, sizeof(SOCKADDR));
	if (SOCKET_ERROR == err)
	{
		closesocket(sock);
		cout << "绑定失败" << endl;
		return false;
	}
	int naddrlen = sizeof(SOCKADDR);
	while (true)
	{
		SOCKADDR_IN addrfrom = { 0 };
		addrfrom.sin_family = AF_INET;
		addrfrom.sin_port = htons(udp_port);
		addrfrom.sin_addr.s_addr = INADDR_BROADCAST;
		char recvbuf[1024] = { 0 };
		int rec = recvfrom(sock, recvbuf, 1024, 0, (SOCKADDR*)&addrfrom, &naddrlen);
		if (recv <= 0)
		{
			cout << "接收错误" << endl;
			continue;
		}
		string ip(inet_ntoa(addrfrom.sin_addr));
		printf("server接收的信息是%s, ip: port=%s:%d\n", recvbuf, ip.c_str(), addrfrom.sin_port);
		int send = sendto(sock, recvbuf, rec, 0, (SOCKADDR*)&addrfrom, naddrlen);
		string str;
		cin >> str;
		const char * ch = str.c_str();
		int sendlen = strlen(ch);
		sendto(sock,ch ,sendlen, 0, (SOCKADDR*)&addrfrom, naddrlen);
		if (send <= 0)
		{
			cout << "发送错误" << endl;
		}
	}
	closesocket(sock);
	return true;

}
int main()
{
	init();
	run();
	finit();
	return 0;

}