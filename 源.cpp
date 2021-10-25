#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")//����������ļ�
#pragma warning(disable:4996)

int main()
{
	//1.��ʼ������
	//�����׽��ֿ�İ汾
	WSADATA wd;
	if (0 != WSAStartup(MAKEWORD(2, 2), &wd)) //WSAStartup����ֵΪ0��ʾ�����ɹ�
	{
		printf("��ʼ������ʧ��.�������:%d\n", GetLastError());//�������10048��ʾ�˿ڱ�ռ��
		return 0;
	};


	//2.�����׽���
	SOCKET sockServ = socket(AF_INET, SOCK_STREAM, 0);
	if (sockServ== INVALID_SOCKET)
	{
		printf("�����׽���ʧ��.�������:%d\n", GetLastError());
		return 0;
	}

	//3.�󶨼����׽���
	SOCKADDR_IN addrServ;
	addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//INADDR_ANY:����ƥ������һ̨���Ե�IP��ַ;  h:host to: n:net l:long
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(2048); //�˿ں�0-65535��0���ã�1-1024��ò�Ҫ�ã�һ��ȡ1024���ϵ�; h:host to: n:net s:short
	
	if (bind(sockServ, (SOCKADDR*)&addrServ, sizeof(addrServ)))
	{
		printf("�����׽���ʧ��.�������:%d\n", GetLastError());
		return 0;
	}

	//4.����
	if (SOCKET_ERROR== listen(sockServ, 5))
	{
		printf("����ʧ��.�������:%d\n", GetLastError());
		return 0;
	};
	
	//5.��������
	while (1)
	{
		//������������
		SOCKADDR_IN clientAddr;
		int length = sizeof(clientAddr);
		SOCKET sockConn = accept(sockServ, (SOCKADDR*)&clientAddr, &length);
		printf("��ӭ�ͻ���:%s:%d���ӷ�����\n", inet_ntoa(clientAddr.sin_addr),clientAddr.sin_port);

		//6.�շ���Ϣ
		while (1)
		{
			printf("\t-->�����뷢�͵���Ϣ����Q�˳���:\n");
			char sendbuf[100];
			scanf("%s", sendbuf);

			if (strcmp(sendbuf, "Q") == 0)
			{
				printf("\t->�����������\n");
				break;
			}

			//��������
			int n=send(sockConn, sendbuf, strlen(sendbuf) + 1, 0);

			//��������
			char recvbuf[100];
			recv(sockConn, recvbuf, sizeof(recvbuf), 0);
			printf("������˵:%s\n", recvbuf);
		}
	}

	
	return 0;
}