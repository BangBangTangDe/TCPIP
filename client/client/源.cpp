#include<winsock2.h>
#include<stdio.h>
#include<iostream>
#include<cstring>
#include<string>
#include"mark.h"
#pragma warning (disable:4996)
using namespace std;
#pragma comment(lib, "ws2_32.lib")

int main()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
	while (true) {
		SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sclient == INVALID_SOCKET)
		{
			printf("�׽��ֳ�ʼ��ʧ��!");
			return 0;
		}

		sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(8888);
		serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{  //����ʧ�� 
			printf("connect error !");
			closesocket(sclient);
			return 0;
		}
		cout << "����Ҫ�������ʽ:" << endl;
		string t;
		cin>>t;
		const char * senddata;
		senddata = t.c_str();
		send(sclient, senddata, strlen(senddata), 0);

		//string data;
		//cin >> data;
		//const char * sendData;
		//sendData = data.c_str();   //stringתconst char* 
		//						   //char * sendData = "��ã�TCP����ˣ����ǿͻ���\n";
		//send(sclient, sendData, strlen(sendData), 0);
		////send()������������ָ����socket�����Է�����
		////int send(int s, const void * msg, int len, unsigned int flags)
		////sΪ�ѽ��������ӵ�socket��msgָ���������ݣ�len��Ϊ���ݳ��ȣ�����flagsһ����0
		////�ɹ��򷵻�ʵ�ʴ��ͳ�ȥ���ַ�����ʧ�ܷ���-1������ԭ�����error 

		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		if (ret>0) {
			recData[ret] = 0x00;
			printf(recData);
		}
		closesocket(sclient);
	}


	WSACleanup();
	return 0;

}
