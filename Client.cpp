#pragma comment(lib,"wsock32.lib")  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <WinSock2.h>  
#include <windows.h>

#define BUFSIZE    512

DWORD WINAPI t_work(void *data)
{


	SOCKET th_sock = (SOCKET)data;
	int t_recv;
	char th_buf[BUFSIZE];
	
	if(recv(th_sock,th_buf,sizeof(th_buf),0) > 0)
		printf("서버 ===>: 클라이언트: %s",th_buf);
		
	
	return 0;
}


int main(int argc, char* argv[]) 
{

	
 if(argc<=2 || argc>3)
	 {
		printf("에코 클라이언트 사용법: 파일이름.exe 서버IP 포트번호\n");
		exit(1);
	 }
	
	int val,echo;
	char * ServerIp = argv[1];
	unsigned short ServerPort = atoi(argv[2]);
	int timeout=1000;

	
	WSADATA wsaData;

	if(WSAStartup(MAKEWORD(2,0),&wsaData) != 0)
	{
		printf("윈속 초기화 에러!!\n");
		exit(1);
	}

	printf("윈속 초기화 성공\n");

	SOCKET client_sock;
	
	client_sock=socket(AF_INET,SOCK_STREAM,0);
	

	if(client_sock == -1)
	{
		printf("클라이언트 소켓 에러\n");
		exit(1);
	}

	SOCKADDR_IN server_addr; 
 
 	ZeroMemory(&server_addr,sizeof(server_addr)); 
 	server_addr.sin_family = AF_INET; 
 	server_addr.sin_addr.s_addr = inet_addr(ServerIp); 
 	server_addr.sin_port = htons(ServerPort); 


 
 	if(connect(client_sock,(SOCKADDR *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
 	{ 
 		printf("서버 접속 에러!!\n"); 
 		exit(1);
	} 
	
	char buf[BUFSIZE+1];
	HANDLE th_thread;

	while(1)
	{
		Sleep(2);
		printf("\n\n전송할 메시지를 입력하시오 (종료시 quit입력): ");
		scanf_s("%s",&buf,BUFSIZE);

		if(strcmp(buf,"quit") == 0)
		{
			printf("프로그램을 종료합니다.\n");
			getchar();
			exit(1);
		}

			val = strlen(buf); 
			if(buf[val-1] == '\n') 
 				buf[val-1] = '\0'; 

			if(send(client_sock,buf,strlen(buf),0) == SOCKET_ERROR)
			{
				printf("Send 에러!!\n");
				exit(1);
			}

		th_thread = CreateThread(NULL,0,t_work,(void *)client_sock,0,NULL);	

			
	}
			
	closesocket(client_sock);

	WSACleanup();

	return 0;
}

