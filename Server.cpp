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
			printf("\n클라이언트와 연결이 종료되었습니다.\n");
			break;
		}

		th_buf[t_recv] = '\0';
		printf("클라이언트 ==> 서버: %s\n",th_buf);

		if(is_echo==1)
			t_send = send(th_sock,th_buf,t_recv,0);
		
		if(t_send == SOCKET_ERROR)
		{
			printf("t_send 오류\n");
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
		printf("에코서버 사용법: 파일이름.exe 포트번호 에코옵션\n");
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
		printf("윈속 초기화 에러!\n");
		exit(1);

	}

 	printf("윈속 초기화 성공!!\n"); 

	
	SOCKET server_sock=socket(AF_INET,SOCK_STREAM,0);
	

	if(server_sock == -1)
	{
		printf("서버 소켓 생성 에러");
		exit(1);

	}
	
	printf("서버 소켓 생성!!\n");

	SOCKADDR_IN serverAddr;
	
	ZeroMemory(&serverAddr,sizeof(serverAddr));

	serverAddr.sin_family=AF_INET;
	serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serverAddr.sin_port=htons(server_port);

	if(bind(server_sock,(SOCKADDR *) &serverAddr,sizeof(serverAddr)) == -1)
	{
		printf("binding 에러!!\n");
		exit(1);
	}

	printf("바인딩 성공!\n");

	if(listen(server_sock,SOMAXCONN) == -1)
	{
		printf("listening 에러!!\n");
		exit(1);
	}

	printf("listen 성공!!\n");

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
			printf("accepting 에러!\n");
			exit(1);
		}

		printf("\n클라이언트와 연결이 되었습니다.\n",count);
		th_thread = CreateThread(NULL,0,t_work,(void *)client_sock,0,NULL);


	}

	closesocket(server_sock);		
 
 		WSACleanup(); 
  
 		printf("윈속 라이브러리 해제!!\n"); 
 		return 0; 
  
 } 
