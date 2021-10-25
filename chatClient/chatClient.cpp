#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")//必须包含的文件

int main()
{
	//1.初始化网络
	//加载套接字库的版本
	WSADATA wd;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) //WSAStartup返回值为0表示创建成功
	{
		printf("初始化网络失败.错误代号:%d\n", GetLastError());//错误代号10048表示端口被占用
		return 0;
	};


	//2.创建套接字
	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == INVALID_SOCKET)
	{
		printf("创建套接字失败.错误代号:%d\n", GetLastError());
		return 0;
	}

	//3.连接服务器

	//inet_nota:
	//inet_addr:

	SOCKADDR_IN addrServ;
	addrServ.sin_addr.S_un.S_addr = inet_addr("192.168.31.250");//INADDR_ANY:可以匹配任意一台电脑的IP地址;  h:host to: n:net l:long
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(2048); //端口号0-65535，0不用，1-1024最好不要用，一般取1024以上的; h:host to: n:net s:short

	if (SOCKET_ERROR == connect(sockClient, (SOCKADDR*)&addrServ, sizeof(addrServ)))
	{
		printf("创建套接字失败.错误代号:%d\n", GetLastError());
		return 0;
	};

	printf("恭喜你，连接成功");

	while (1)
	{
		//接受信息
		char recvbuf[100];
		int n = recv(sockClient, recvbuf, sizeof(recvbuf), 0);
		recvbuf[n] = '\0';
		printf("服务器说:%s\n",recvbuf);

		//发送数据
		printf("\t-->请输入发送的信息【按Q退出】:\n");
		char sendbuf[100];
		scanf("%s", sendbuf);

		if (strcmp(sendbuf, "Q") == 0)
		{
			printf("\t->【聊天结束】\n");
			break;
		}

		//发送数据
		send(sockClient, sendbuf, strlen(sendbuf) + 1, 0);

	}

	return 0;
}