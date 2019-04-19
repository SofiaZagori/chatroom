#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>



int main(int argc,char *argv[]){
	int sock,i;
	char msg[1024],name[50];
	struct sockaddr_in addr;
	
	addr.sin_family=AF_INET;
	inet_aton("127.0.0.1",&addr.sin_addr); //localhost
	addr.sin_port=htons(1111);
	sock=socket(AF_INET,SOCK_STREAM,0); //stream socket
	connect(sock,(struct sockaddr *)&addr,sizeof(addr));
	printf("connected, pls give nickname\n");

	fgets(name,50,stdin);
	name[strlen(name)-1]='\0';
	send(sock,name,strlen(name)+1,0);

	while(strcmp(msg,"exit") !=0){
		strcpy(msg,"");
		printf("write something\n");
		fgets(msg,1024,stdin);	
		msg[strlen(msg)-1]='\0';
		printf("sending command:%s\n",msg);
		send(sock,msg,strlen(msg)+1,0);
	}


	close(sock);

return 0;
}