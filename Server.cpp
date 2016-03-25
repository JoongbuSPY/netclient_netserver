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
		printf("���ڼ��� ����: �����̸�.exe ��Ʈ��ȣ ���ڿɼ�\n");
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

	char y_echo[]={0};//0�̸� ���ڱ������
	char n_echo[]={1};//1�̸� ���ڱ�� ����.
	
	while(1)
	{
		printf("\n����ϰ� �ִ� Ŭ���̾�Ʈ�� �����ϴ�.\n");

		client_len=sizeof(clientAddr);

		client_sock = accept(server_sock,(SOCKADDR *) &clientAddr,&client_len);

		if(client_sock == -1)
		{
			printf("accepting ����!\n");
			exit(1);
		}

		if(is_echo==true)
			send(client_sock,y_echo,sizeof(y_echo),0);
		else
			send(client_sock,n_echo,sizeof(n_echo),0);

		printf("Ŭ���̾�Ʈ�� ������ �Ǿ����ϴ�.\n");

		while(1)
		{
			val=recv(client_sock,buf,BUFSIZE,0);
			
			if(val ==  SOCKET_ERROR || val==0 )
			{
				printf("recving ����!\n");
				exit(1);
			}
			
			buf[val] = '\0';
			printf("Ŭ���̾�Ʈ ==> ����: %s\n",buf);
			
			if(is_echo == true)
				send(client_sock,buf,sizeof(buf),0);

		}
		closesocket(client_sock);

	}

	closesocket(server_sock);		
 
 		WSACleanup(); 
  
 		printf("���� ���̺귯�� ����!!\n"); 
 		return 0; 
  
  
 } 
