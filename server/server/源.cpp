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
	//��ʼ��WSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//�����׽���  
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//��IP�Ͷ˿�  
	sockaddr_in sin;//����ṹ���ַ
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);//���ö˿�
	sin.sin_addr.S_un.S_addr = INADDR_ANY;//��������ַȡ�����κο��õ�ַ
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)//��ַ���׽��ְ�
	{
		printf("bind error !");
	}

	//��ʼ����  
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������  
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		printf("�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf(" !");
			continue;
		}
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
		//����ת�������ַΪʮ���Ƶ�

		//��������  
		string t;
		arithmetic ans;
		int ret = recv(sClient, revData, 255, 0);
		if (ret > 0)
		{
			revData[ret] =0x00;
			printf("%s", revData);
			t = revData;
			ans = conform(t);
			cout << "����Ľ����" << cal(ans) << endl;
			cout << "�Ѿ���ͻ��˷����˼���Ľ��" << endl;

		}
		char buf[20];
		sprintf(buf, "%.3f", cal(ans));
		//��������  
		int a = strlen(buf);
		send(sClient, buf, strlen(buf), 0);
		closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}