#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#define BufferSize 10000

int main(int argc, char* argv[])
{
	if(argc!=3) {
		printf("Usage Error : ./client (Server_IP_Addr) (Server_Port_Num) \n");
		return 0;
	}
	int clientSocket= socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sAddr;
	int clientFlag = 0;
	if(clientSocket==-1) {
		printf("Socket Creation Error! \n");
		return 0;
	}
	else printf("Socket Created!\n");

	bzero(&sAddr, sizeof(sAddr));
	
	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = inet_addr(argv[1]);
	sAddr.sin_port = htons(atoi(argv[2]));

	if(connect(clientSocket, (struct sockaddr*) &sAddr, sizeof(sAddr)) == -1) {
		printf("Error : Server Connect Error\n");
		close(clientSocket);
		return 0;
	}
	else printf("Server Connect Success!\n");

	char Buff[BufferSize];
	bzero(Buff, sizeof(Buff));
	recv(clientSocket, Buff, BufferSize, 0);


	if(strlen(Buff)==0) {
		printf("Client Ready Error\n");
		close(clientSocket);
		return 0;
	}
	else {
		if(strncmp(Buff, "READY: You are a sender",23)==0) {
			clientFlag=1;

		}
		else clientFlag=2;
	}

	bzero(Buff, BufferSize);
	if(clientFlag==1) {
		printf("Send Message: ");
		scanf("%[^\n]s", Buff);
		if(send(clientSocket,Buff, BufferSize, 0)<0) {
			printf("Send Failed\n");
			close(clientSocket);
	                return 0;
		}	
		else {
			printf("Send Successed!\n");
		}
	}
	else {
		if(recv(clientSocket, Buff, BufferSize, 0)<0) {
			printf("Receive Failed\n");
			close(clientSocket);
			return 0;
		}
		printf("Received Message: %s\n", Buff);
	}

	printf("Successed!\n");
	close(clientSocket);
	
	return 0;

}
