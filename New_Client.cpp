#pragma comment(lib,"wsock32.lib")// TCP/IP���� ���Ǵ� �Լ��� �Ϲ����� C ���̺귯���� �ƴϱ� ������ wsock32.lib�� �ݵ�� ������ �־�� �ϴµ� #pragma comment ���� Ư�� ���̺귯�� ������ ���Խ�ų �� ����Ѵ�.
#include <winsock2.h>// TCP/IP���� ���Ǵ� ��� �Լ����� winsock2.h�� ����Ǿ� �ֽ��ϴ�.
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE    512

int main(int argc, char* argv[])
{
	int val;
	char * ServerIp = argv[1];
	unsigned short ServerPort = atoi(argv[2]);


	WSADATA wd;

	if(WSAStartup(MAKEWORD(2,0),&wd) != 0)
	{
		printf("���� �ʱ�ȭ ����!!\n");
		return 0;
	}

	printf("���� �ʱ�ȭ ����!!\n");

	SOCKET csock=socket(AF_INET,SOCK_STREAM,0);

	if(csock == INVALID_SOCKET)
	{
		printf("Ŭ���̾�Ʈ ���� ���� ����!!\n");
		return 0;
	}

	printf("Ŭ���̾�Ʈ ���� ����!!\n");

	SOCKADDR_IN saddr;

	ZeroMemory(&saddr,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr =  inet_addr(ServerIp);
	saddr.sin_port = htons(ServerPort);

	if(connect(csock, (SOCKADDR *)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		printf("���� ���� ����!!\n");
		return 0;
	}

	char buf[BUFSIZE+1];
	int blen;

	while(1)
	{

		printf("�޼���: ");
		
		if(fgets(buf,BUFSIZE+1,stdin)==NULL)
			break;

		blen = strlen(buf);
		if(buf[blen-1] == '\n')
			buf[blen-1] = '\0';

		if(strlen(buf)==0)
			break;

		val=send(csock,buf,strlen(buf),0);

		if(val == SOCKET_ERROR)
		{
			printf("�������� ���� �� �� �����ϴ�.!!\n");
			break;
		}

		if(strcmp(buf, "quit") == 0)
		{
			printf("������ �����մϴ�.!!\n");
			break;
		}
		
		if(argc == 4)
		{
			val=recv(csock,buf,sizeof(buf)-1,0);
			printf("���� => Ŭ���̾�Ʈ: %s \n",buf);
		}
			
		
	}

	closesocket(csock);

	WSACleanup();
	return 0;

}