#pragma comment(lib,"wsock32.lib") 
 #include <stdio.h> 
 #include <stdlib.h> 
 #include <string.h> 
 #include <WinSock2.h> 
  
 #define BUFSIZE 512 
  


 int main(int argc, char* argv[]) 
 { 
		
	 if(argc==1)
	 {
		printf("���ڼ��� ����: �����̸�.exe ��Ʈ��ȣ ���ڿɼ�\n");
		exit(1);
	 }
			 
 
 	WSADATA wsadata;
	int val;
	char buf[BUFSIZE];
	
  
 	if(WSAStartup(MAKEWORD(2,0),&wsadata) != 0)
		printf("���� �ʱ�ȭ ����!\n");

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
	serverAddr.sin_port=htons(atoi(argv[1]));

	if(bind(server_sock,(SOCKADDR *) &serverAddr,sizeof(serverAddr)) == -1)
	{
		printf("binding ����!!\n");
		exit(1);
	}

	if(listen(server_sock,5)==-1)
	{
		printf("listening ����!!\n");
		exit(1);
	}

	SOCKET client_sock;
	SOCKADDR_IN clientAddr;
	int client_addr_len;
	while(1)
	{
		printf("����ϰ� �ִ� Ŭ���̾�Ʈ�� �����ϴ�.\n");
		client_addr_len=sizeof(clientAddr);
		if(accept(server_sock,(SOCKADDR *) &clientAddr,&client_addr_len) == -1)
		{
			printf("accepting ����!\n");
			exit(1);
		}

		printf("Ŭ���̾�Ʈ�� ������ �Ǿ����ϴ�.\n");

		while(1)
		{
			val=recv(client_sock,buf,BUFSIZE,0);
			
			if(val ==  SOCKET_ERROR )
			{
				printf("recving ����!\n");
				exit(1);
			}
			
			buf[val] = '\0';
			printf("Ŭ���̾�Ʈ ==> ����: %s\n",buf);

			if(strcmp(argv[2],"-echo") == 0)
				send(client_sock,buf,sizeof(buf),0);

		}
		closesocket(client_sock);

	}

	closesocket(server_sock);		
 
 		WSACleanup(); 
  
 		printf("���� ���̺귯�� ����!!\n"); 
 		return 0; 
  
  
 } 
