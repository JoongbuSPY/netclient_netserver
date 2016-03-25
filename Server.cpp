#pragma comment(lib,"wsock32.lib") 
 #include <stdio.h> 
 #include <stdlib.h> 
 #include <string.h> 
 #include <WinSock2.h> 
  
 #define BUFSIZE 512
#define NOBUF 0
  


 int main(int argc, char* argv[]) 
 { 
		
	 bool is_echo=false;

	 if(argc<0 || argc>3)
	 {
		printf("에코서버 사용법: 파일이름.exe 포트번호 에코옵션\n");
		exit(1);
	 }

	if(argc==3)
	{
		if(strcmp(argv[2],"-echo") == 0)
			is_echo=true;
	}
		
	 
 	WSADATA wsadata;
	int val;
	unsigned short server_port=atoi(argv[1]);
	char buf[BUFSIZE];
	
  
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

	char y_echo[]={0};//0이면 에코기능켜짐
	char n_echo[]={1};//1이면 에코기능 꺼짐.
	
	while(1)
	{
		printf("\n대기하고 있는 클라이언트가 없습니다.\n");

		client_len=sizeof(clientAddr);

		client_sock = accept(server_sock,(SOCKADDR *) &clientAddr,&client_len);

		if(client_sock == -1)
		{
			printf("accepting 에러!\n");
			exit(1);
		}

		if(is_echo==true)
			send(client_sock,y_echo,sizeof(y_echo),0);
		else
			send(client_sock,n_echo,sizeof(n_echo),0);

		printf("클라이언트와 연결이 되었습니다.\n");

		while(1)
		{
			val=recv(client_sock,buf,BUFSIZE,0);
			
			if(val ==  SOCKET_ERROR || val==0 )
			{
				printf("recving 에러!\n");
				exit(1);
			}
			
			buf[val] = '\0';
			printf("클라이언트 ==> 서버: %s\n",buf);
			
			if(is_echo == true)
				send(client_sock,buf,sizeof(buf),0);

		}
		closesocket(client_sock);

	}

	closesocket(server_sock);		
 
 		WSACleanup(); 
  
 		printf("윈속 라이브러리 해제!!\n"); 
 		return 0; 
  
  
 } 
