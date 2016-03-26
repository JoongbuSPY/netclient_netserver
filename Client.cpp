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
		printf("���� ===>: Ŭ���̾�Ʈ: %s",th_buf);
		
	
	return 0;
}

int main(int argc, char* argv[]) 
{
	
	int val,echo;
	char * ServerIp = argv[1];
	unsigned short ServerPort = atoi(argv[2]);
	int timeout=1000;

	if(argc != 3)
	{
		printf("���ϸ�.exe ����IP ����PORT �� �Է��ϼ���\n");
		exit(1);
	}

	
	WSADATA wsaData;

	if(WSAStartup(MAKEWORD(2,0),&wsaData) != 0)
	{
		printf("���� �ʱ�ȭ ����!!\n");
		exit(1);
	}

	printf("���� �ʱ�ȭ ����\n");

	SOCKET client_sock;
	
	client_sock=socket(AF_INET,SOCK_STREAM,0);
	

	if(client_sock == -1)
	{
		printf("Ŭ���̾�Ʈ ���� ����\n");
		exit(1);
	}

	SOCKADDR_IN server_addr; 
 
 	ZeroMemory(&server_addr,sizeof(server_addr)); 
 	server_addr.sin_family = AF_INET; 
 	server_addr.sin_addr.s_addr = inet_addr(ServerIp); 
 	server_addr.sin_port = htons(ServerPort); 


 
 	if(connect(client_sock,(SOCKADDR *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
 	{ 
 		printf("���� ���� ����!!\n"); 
 		exit(1);
	} 
	
	char buf[BUFSIZE+1];
	HANDLE th_thread;

	while(1)
	{
		Sleep(2);
		printf("\n\n������ �޽����� �Է��Ͻÿ� (����� quit�Է�): ");
		scanf_s("%s",&buf,BUFSIZE);

		if(strcmp(buf,"quit") == 0)
		{
			printf("���α׷��� �����մϴ�.\n");
			getchar();
			exit(1);
		}

			val = strlen(buf); 
			if(buf[val-1] == '\n') 
 				buf[val-1] = '\0'; 

			if(send(client_sock,buf,strlen(buf),0) == SOCKET_ERROR)
			{
				printf("Send ����!!\n");
				exit(1);
			}

		th_thread = CreateThread(NULL,0,t_work,(void *)client_sock,0,NULL);	

			
	}
			
	closesocket(client_sock);

	WSACleanup();

	return 0;
}

