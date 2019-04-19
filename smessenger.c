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

char msg[1024];

typedef struct {
	char name[50];
	int id;		//thread
	int sock;
}Client;


void client_handler(void *sock){
	Client newclient;
	newclient.sock=*(int*)sock;
	
	printf("mesa sto thread pairnei onoma\n");
	recvfrom(newclient.sock,newclient.name,50,0,NULL,NULL);
	printf("%s is online\n",newclient.name);
	while(strcmp(msg,"exit") !=0){
			recvfrom(newclient.sock,msg,1024,0,NULL,NULL);
			if(strcmp(msg,"exit")!=0){
				printf("%s send: %s\n",newclient.name,msg);
			}
		}
}

int main(int argc,char *argv[]){

	int psock,sock,c,ret,i;
	
	struct sockaddr_in addr; //internet socket
	
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);//all interfaces
	addr.sin_port=htons(1111);
	psock=socket(AF_INET,SOCK_STREAM,0);
	bind(psock,(struct sockaddr *)&addr,sizeof(addr));
	listen(psock,10);
	c = sizeof(struct sockaddr_in);
	pthread_t id;
	printf("\nwaiting for new connection...\n");
	while(sock = accept(psock,NULL,NULL)){

		strcpy(msg,"");
		printf("connection established...\n");
		ret=pthread_create(&id, NULL,(void*)client_handler,(void*)&sock);
		i++;
		if(i==10){
			break;
		}
	}
	close(sock);
	printf("\nconnection closed\n");
        
        pthread_join(id,NULL);

	close(psock);
        
return 0;
}