#pragma comment(lib,"wsock32.lib")
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUFSIZE 512

int main(int argc, char* argv[])
{
	int val;
	unsigned short ServerPort;

	ServerPort = atoi( argv[1] );

	WSADATA wd;

	if(WSAStartup(MAKEWORD(2,0),&wd) != 0)
	{
		printf("WinSock Error!!\n");
		return 0;
	}
	printf("윈속 초기화 성공!!\n");

	SOCKET ser_sock=socket(AF_INET,SOCK_STREAM,0);
	
	if(ser_sock == INVALID_SOCKET)
	{
		printf("SocKet Error!!\n");
		return 0;
	}

	printf("서버 소켓 생성!!\n");

	SOCKADDR_IN ser_addr;

	ZeroMemory(&ser_addr,sizeof(ser_addr));

	ser_addr.sin_family=AF_INET;
	ser_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	ser_addr.sin_port=htons(ServerPort);

	
	if(bind(ser_sock,(SOCKADDR *)&ser_addr,sizeof(ser_addr)) == SOCKET_ERROR)
	{
		printf("바인딩 실패!!\n");
		return 0;
	}

	printf("서버 소켓 주소할당 성공!\n");

	if(listen(ser_sock,SOMAXCONN) == SOCKET_ERROR)
	{
		printf("연결 요청한 클라이언트가 없습니다.\n");
		return 0;
	}

		SOCKET cli_sock;
		SOCKADDR_IN cli_addr;
		int addrlen;
		char buf[BUFSIZE];


	while(1)
	{
		printf("연결 요청 대기 상태!!\n");
		addrlen=sizeof(cli_addr);

		cli_sock = accept(ser_sock,(SOCKADDR *)&cli_addr,&addrlen);

		if(cli_sock == INVALID_SOCKET)
		{
			printf("연결 요청이 실패하였습니다.!!\n");
			break;
		}

		printf("연결 요청 수락!!\n");

		while(1)
		{
			val = recv(cli_sock,buf,BUFSIZE,0);

			if(strcmp(buf, "quit") == 0)
			{
				printf("\n클라이언트의 요청으로 접속을 종료합니다.");
				getchar();
				exit(1);
			}

			else if(val == 0)
			{
				printf("\n입력한 내용이 없습니다. 프로그램을 종료합니다.");
				getchar();
				exit(1);
			}

			buf[val] = '\0';
			printf("메세지: %s\n",buf);

			send(cli_sock,buf,sizeof(buf),0);

		}

		closesocket(cli_sock);
		printf("클라이언트와 연결 종료!!\n");
	
	}

		closesocket(ser_sock);
		WSACleanup();

		printf("윈속 라이브러리 해제!!\n");
		return 0;


}