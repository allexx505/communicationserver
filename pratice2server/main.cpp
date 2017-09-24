#include<iostream>
#include<stdlib.h>
#include"winsock2.h"
#include<string>
#pragma comment(lib,"ws2_32.lib")		//引用头文件以及库文件

using namespace std;		//使用标准类库命名空间
//多线程,处理接受信息
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
	bindaddr.sin_addr.S_un.S_addr = INADDR_ANY;//设定服务端数据输入口地址以及端口号

	int i = bind(hsock, (sockaddr*)&bindaddr, sizeof(bindaddr));
	cout << i << endl;

	sockaddr addrfrom;
	int len = sizeof(addrfrom);

	while (1)
	{
		memset(buffer, 0, 1024);
		num = recvfrom(hsock, buffer, sizeof(buffer), 0, &addrfrom, &len);
		if (num >= 0)
			cout << endl << buffer << endl;//接受信息
	}
}

//主函数，处理发送信息
void main()
{
	WSADATA wsd;
	WSAStartup(MAKEWORD(2, 2), &wsd);//定义WSA对象以及初始化	
	SOCKET mserver;
	mserver = socket(AF_INET, SOCK_DGRAM, 0);		//定义套接字，以及设置套接字的模式
	char addrbuf[1024];
	cout << "请输入IP:" << endl;
	cin >> addrbuf;

	sockaddr_in serveraddr;				
	serveraddr.sin_family = AF_INET;			//定义服务器家族链接方式
	serveraddr.sin_port = htons(4600);			//定义链接端口
	serveraddr.sin_addr.S_un.S_addr = inet_addr(addrbuf);//定义链接的IP地址，本机地址

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