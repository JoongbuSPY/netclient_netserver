#pragma comment(lib,"wsock32.lib")// TCP/IP에서 사용되는 함수는 일반적인 C 라이브러리가 아니기 때문에 wsock32.lib를 반드시 포함해 주어야 하는데 #pragma comment 문은 특정 라이브러리 파일을 포함시킬 때 사용한다.
#include <winsock2.h>// TCP/IP에서 사용되는 모든 함수들은 winsock2.h에 선언되어 있습니다.
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
		printf("윈속 초기화 에러!!\n");
		return 0;
	}

	printf("윈속 초기화 성공!!\n");

	SOCKET csock=socket(AF_INET,SOCK_STREAM,0);

	if(csock == INVALID_SOCKET)
	{
		printf("클라이언트 소켓 생성 에러!!\n");
		return 0;
	}

	printf("클라이언트 소켓 생성!!\n");

	SOCKADDR_IN saddr;

	ZeroMemory(&saddr,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr =  inet_addr(ServerIp);
	saddr.sin_port = htons(ServerPort);

	if(connect(csock, (SOCKADDR *)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		printf("서버 접속 에러!!\n");
		return 0;
	}

	char buf[BUFSIZE+1];
	int blen;

	while(1)
	{

		printf("메세지: ");
		
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
			printf("서버에게 전달 할 수 없습니다.!!\n");
			break;
		}

		if(strcmp(buf, "quit") == 0)
		{
			printf("접속을 종료합니다.!!\n");
			break;
		}
		
		if(argc == 4)
		{
			val=recv(csock,buf,sizeof(buf)-1,0);
			printf("서버 => 클라이언트: %s \n",buf);
		}
			
		
	}

	closesocket(csock);

	WSACleanup();
	return 0;

}