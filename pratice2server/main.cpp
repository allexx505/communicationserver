#include<iostream>
#include<stdlib.h>
#include"winsock2.h"
#include<string>
#pragma comment(lib,"ws2_32.lib")		//����ͷ�ļ��Լ����ļ�

using namespace std;		//ʹ�ñ�׼��������ռ�
//���߳�,���������Ϣ
DWORD WINAPI threadpro(LPVOID pParam)
{
	//SOCKET hsock = (SOCKET)pParam;
	SOCKET hsock;
	char buffer[1024];
	int num;
	hsock = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (hsock != INVALID_SOCKET)
		cout << "start receive" << endl;

	sockaddr_in bindaddr;
	bindaddr.sin_family = AF_INET;
	bindaddr.sin_port = htons(4601);
	bindaddr.sin_addr.S_un.S_addr = INADDR_ANY;//�趨�������������ڵ�ַ�Լ��˿ں�

	int i = bind(hsock, (sockaddr*)&bindaddr, sizeof(bindaddr));
	cout << i << endl;

	sockaddr addrfrom;
	int len = sizeof(addrfrom);

	while (1)
	{
		memset(buffer, 0, 1024);
		num = recvfrom(hsock, buffer, sizeof(buffer), 0, &addrfrom, &len);
		if (num >= 0)
			cout << endl << buffer << endl;//������Ϣ
	}
}

//����������������Ϣ
void main()
{
	WSADATA wsd;
	WSAStartup(MAKEWORD(2, 2), &wsd);//����WSA�����Լ���ʼ��	
	SOCKET mserver;
	mserver = socket(AF_INET, SOCK_DGRAM, 0);		//�����׽��֣��Լ������׽��ֵ�ģʽ
	char addrbuf[1024];
	cout << "������IP:" << endl;
	cin >> addrbuf;

	sockaddr_in serveraddr;				
	serveraddr.sin_family = AF_INET;			//����������������ӷ�ʽ
	serveraddr.sin_port = htons(4600);			//�������Ӷ˿�
	serveraddr.sin_addr.S_un.S_addr = inet_addr(addrbuf);//�������ӵ�IP��ַ��������ַ

	HANDLE mhandle;
	DWORD nthreadid = 0;
	char headmessage[] = "from server:  ";
	char buffer[1024];

	mhandle = (HANDLE)::CreateThread(0, 0, threadpro, (LPVOID)mserver, 0, &nthreadid);
	while (1)
	{
		cout << "TO:";
		cin.getline(buffer,1024);
		int numsend = sendto(mserver, headmessage, 1024, 0, (sockaddr*)&serveraddr, sizeof(serveraddr));
		numsend = sendto(mserver, buffer, 1024, 0, (sockaddr*)&serveraddr, sizeof(serveraddr));
		cout << numsend << endl;
	}
}