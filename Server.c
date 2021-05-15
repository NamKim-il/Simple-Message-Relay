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
	if(argc!=2) {
		printf("Usage Error : ./server (PortNumber)\n");
		return 0;
	}
	int senderSocket, receiverSocket, serverSocket;
	struct sockaddr_in sAddr, sendAddr, recvAddr;
	socklen_t clientLen=sizeof(sendAddr);
	bzero(&sAddr, sizeof(sAddr));
	char Buff[BufferSize];

	if( (serverSocket=socket(PF_INET, SOCK_STREAM, 0)) == -1 ){
	       	printf("Socket Creation Error!\n");
 		return 0;
	}		

	sAddr.sin_family=AF_INET;
	sAddr.sin_port=htons(atoi(argv[1]));   
	sAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	if(bind(serverSocket,(struct sockaddr*)&sAddr,sizeof(sAddr))==-1) {		
		printf("Failed at binding\n");
		close(serverSocket);
		return 0;
	}
	if(listen(serverSocket,SOMAXCONN)==-1) {
		printf("Failed at Listen\n");
      		close(serverSocket);
      		return 0;
   	}
	printf("Listening port : %s\n", argv[1]); 
	
	senderSocket = accept(serverSocket,(struct sockaddr*)&sendAddr, &clientLen);
	if(clientLen==-1) {
		printf("Failed to Accept!(Sender)\n");
		close(serverSocket);
		return 0;
	}

	clientLen=sizeof(receiverSocket);
	receiverSocket = accept(serverSocket, (struct sockaddr*)&recvAddr, &clientLen);
	if(clientLen==-1) {
		printf("Failed to Accept!(Receiver)\n");
		close(serverSocket);
		return 0;
	}

	if( send(senderSocket, "READY: You are a sender.", strlen("READY: You are a sender."), 0) <0 ) {
		printf("Failed to Send READY Message (Sender) \n");
		close(serverSocket);
		return 0;
	}

	if( send(receiverSocket, "READY: You are a receiver.", strlen("READY: You are a receiver."), 0) <0 ) {
                printf("Failed to Send READY Message (receiver) \n");
                close(serverSocket);
                return 0;
        }
	
	bzero(Buff, sizeof(Buff));
	if( recv(senderSocket, Buff, BufferSize, 0)<=0 )
	{
		printf("Failed to Receive Message From Sender\n"); 
		close(serverSocket);
		return 0;
	}

	if( send(receiverSocket, Buff, strlen(Buff), 0) <= 0) {
		printf("Failed to Send Message To Receiver\n");
		close(serverSocket);
		return 0;
	}

	printf("TCP connection complete\n");
	close(serverSocket);
	return 0;

}
