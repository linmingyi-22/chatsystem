#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")//必须包含的文件
#pragma warning(disable:4996)

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
	SOCKET sockServ = socket(AF_INET, SOCK_STREAM, 0);
	if (sockServ== INVALID_SOCKET)
	{
		printf("创建套接字失败.错误代号:%d\n", GetLastError());
		return 0;
	}

	//3.绑定监听套接字
	SOCKADDR_IN addrServ;
	addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY:可以匹配任意一台电脑的IP地址;  h:host to: n:net l:long
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(2048); //端口号0-65535，0不用，1-1024最好不要用，一般取1024以上的; h:host to: n:net s:short
	
	if (bind(sockServ, (SOCKADDR*)&addrServ, sizeof(addrServ)))
	{
		printf("创建套接字失败.错误代号:%d\n", GetLastError());
		return 0;
	}

	//4.监听
	if (SOCKET_ERROR== listen(sockServ, 5))
	{
		printf("监听失败.错误代号:%d\n", GetLastError());
		return 0;
	};
	
	//5.建立连接
	while (1)
	{
		//建立连接请求
		SOCKADDR_IN clientAddr;
		int length = sizeof(clientAddr);
		SOCKET sockConn = accept(sockServ, (SOCKADDR*)&clientAddr, &length);
		printf("欢迎客户端:%s:%d连接服务器\n", inet_ntoa(clientAddr.sin_addr),clientAddr.sin_port);

		//6.收发信息
		while (1)
		{
			printf("\t-->请输入发送的信息【按Q退出】:\n");
			char sendbuf[100];
			scanf("%s", sendbuf);

			if (strcmp(sendbuf, "Q") == 0)
			{
				printf("\t->【聊天结束】\n");
				break;
			}

			//发送数据
			int n=send(sockConn, sendbuf, strlen(sendbuf) + 1, 0);

			//接受数据
			char recvbuf[100];
			recv(sockConn, recvbuf, sizeof(recvbuf), 0);
			printf("服务器说:%s\n", recvbuf);
		}
	}

	
	return 0;
}