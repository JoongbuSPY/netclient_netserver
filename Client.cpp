#pragma comment(lib,"wsock32.lib")// TCP/IP���� ���Ǵ� �Լ��� �Ϲ����� C ���̺귯���� �ƴϱ� ������ wsock32.lib�� �ݵ�� ������ �־�� �ϴµ� #pragma comment ���� Ư�� ���̺귯�� ������ ���Խ�ų �� ����Ѵ�.
#include <winsock2.h>// TCP/IP���� ���Ǵ� ��� �Լ����� winsock2.h�� ����Ǿ� �ֽ��ϴ�.
#include <stdio.h>
#include <stdlib.h>


#define SERVERIP   "127.0.0.1"
#define SERVERPORT 1234
#define BUFSIZE    512

int main(void)
{
	int val;

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
	saddr.sin_addr.s_addr = inet_addr(SERVERIP);
	saddr.sin_port = htons(SERVERPORT);

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
		val=recv(csock,buf,sizeof(buf)-1,0);
		
		if(val == 0)
			break;
		printf("���� => Ŭ���̾�Ʈ: %s \n",buf);
	
	}

	closesocket(csock);

	WSACleanup();
	return 0;





}