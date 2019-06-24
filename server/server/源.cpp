#include <stdio.h>  
#include<iostream>
#include <winsock2.h>  
#include<string.h>
#include<string>
#pragma warning(disable:4996)
#pragma comment(lib,"ws2_32.lib")  
#include"mark.h"
using namespace std;
double cal(arithmetic t)
{
	if (t.mark == '+')
		return t.data1 + t.data2;
	else if (t.mark == '-')
		return t.data1 - t.data2;
	else if (t.mark == '*')
		return t.data1*t.data2;
	else if (t.mark == '/')
		return t.data1 / t.data2;
	return 0;
}
arithmetic conform(string str)
{
	arithmetic temp;
	int len = str.size();
	string a = ""; char b;
	double x, y;
	
	for (int i = 0; i < len; i++)
	{
		if (str[i] >= '0'&&str[i] <= '9')
			a += str[i];
		else
		{
			x = atof(a.c_str());
			temp.data1 = x;
			a = "";
			b = str[i];
			temp.mark = b;
		}
	}
	y = atof(a.c_str());
	temp.data2 = y;
	return temp;
}

int main()
{

	arithmetic my = conform("12*50");

	double a = cal(my);
	//初始化WSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字  
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口  
	sockaddr_in sin;//定义结构体地址
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);//设置端口
	sin.sin_addr.S_un.S_addr = INADDR_ANY;//服务器地址取本机任何可用地址
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)//地址和套接字绑定
	{
		printf("bind error !");
	}

	//开始监听  
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据  
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		printf("等待连接...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf(" !");
			continue;
		}
		printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
		//函数转换网络地址为十进制点

		//接收数据  
		string t;
		arithmetic ans;
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] =0x00;
			printf("%s", revData);
			t = revData;
			ans = conform(t);
			cout << "计算的结果是" << cal(ans) << endl;
			cout << "已经向客户端返回了计算的结果" << endl;

		}
		char buf[20];
		sprintf(buf, "%.3f", cal(ans));
		//发送数据  
		int a = strlen(buf);
		send(sClient, buf, strlen(buf), 0);
		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}