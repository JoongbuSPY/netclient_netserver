#pragma comment(lib,"wsock32.lib") 
 #include <stdio.h> 
 #include <stdlib.h> 
 #include <string.h> 
 #include <WinSock2.h> 
  
 #define BUFSIZE 512

int is_echo=0;
int count=1;

DWORD WINAPI t_work(void *data)
{

	SOCKET th_sock = (SOCKET)data;
	int t_send,t_recv;
	char th_buf[BUFSIZE];

	ZeroMemory(&th_buf,sizeof(th_buf));

	while(t_recv = recv(th_sock,th_buf,BUFSIZE,0))
	{
		if(t_recv == SOCKET_ERROR || t_recv == 0)
		{
			printf("\nŬ���̾�Ʈ�� ������ ����Ǿ����ϴ�.\n");
			break;
		}

		th_buf[t_recv] = '\0';
		printf("Ŭ���̾�Ʈ ==> ����: %s\n",th_buf);

		if(is_echo==1)
			t_send = send(th_sock,th_buf,t_recv,0);
		
		if(t_send == SOCKET_ERROR)
		{
			printf("t_send ����\n");
			break;
		}

	ZeroMemory(&th_buf,sizeof(th_buf));
	}
	closesocket(th_sock);
	
	return 0;
}


 int main(int argc, char* argv[]) 
 { 

	 if(argc<0 || argc>3)
	 {
		printf("���ڼ��� ����: �����̸�.exe ��Ʈ��ȣ ���ڿɼ�\n");
		exit(1);
	 }

	 if(argc==3)
	{
		if(strcmp(argv[2],"-echo") == 0)
			is_echo=1;
	}
		
 	WSADATA wsadata;
	unsigned short server_port=atoi(argv[1]);

	
  
 	if(WSAStartup(MAKEWORD(2,0),&wsadata) != 0)
	{
		printf("���� �ʱ�ȭ ����!\n");
		exit(1);

	}

 	printf("���� �ʱ�ȭ ����!!\n"); 

	
	SOCKET server_sock=socket(AF_INET,SOCK_STREAM,0);
	

	if(server_sock == -1)
	{
		printf("���� ���� ���� ����");
		exit(1);

	}
	
	printf("���� ���� ����!!\n");

	SOCKADDR_IN serverAddr;
	
	ZeroMemory(&serverAddr,sizeof(serverAddr));

	serverAddr.sin_family=AF_INET;
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddr.sin_port=htons(server_port);

	if(bind(server_sock,(SOCKADDR *) &serverAddr,sizeof(serverAddr)) == -1)
	{
		printf("binding ����!!\n");
		exit(1);
	}

	printf("���ε� ����!\n");

	if(listen(server_sock,SOMAXCONN) == -1)
	{
		printf("listening ����!!\n");
		exit(1);
	}

	printf("listen ����!!\n");

	SOCKET client_sock;
	SOCKADDR_IN clientAddr;
	int client_len;
	HANDLE th_thread;

	
	while(1)
	{
		
		client_len=sizeof(clientAddr);

		client_sock = accept(server_sock,(SOCKADDR *) &clientAddr,&client_len);

		if(client_sock == -1)
		{
			printf("accepting ����!\n");
			exit(1);
		}

		printf("\nŬ���̾�Ʈ�� ������ �Ǿ����ϴ�.\n",count);
		th_thread = CreateThread(NULL,0,t_work,(void *)client_sock,0,NULL);


	}

	closesocket(server_sock);		
 
 		WSACleanup(); 
  
 		printf("���� ���̺귯�� ����!!\n"); 
 		return 0; 
  
 } 
