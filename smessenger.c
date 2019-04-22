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

//global vars			global metavlites
char msg[1024];			// to mhnyma poy tha stelnei o kathenas client
int clientCount = 0;		//metritis gia to posous clients ekshphretoyme
int sock_ids[10];		//pinakas poy mazeyoyme ta sockets gia na mporoyme na steiloyme meta se olous

typedef struct {		//struct me antikeimeno CLIENT
	char name[50];		//enas client xarakthrizetai apo onoma kai socket
	int sock;
}Client;

void send_to_all(int currentSock, char *msg, char *sender){	//stelnei se olous to mhnyma
	char temp[50];						//metavlith gia na perigrafei to mhnyma
	strcpy(temp, "");
	strcpy(temp, sender);			//periexei ton apostolea kai to mhnyma poy esteile
	strcat(temp, " said ");
	strcat(temp, msg);
	for(int i=0; i< clientCount; i++){	//me mia for pernaei ola ta sockets ektos apo ton apostolea
		if(sock_ids[i] != currentSock){
			send(sock_ids[i], temp, 1024, 0);
		}	
	}
}
void someone_is_online(int currentSock, char *sender){	//paromoio me thn parapanw synarthsh mono poy den exei msg
	char temp[50];					//enhmerwnei toys alloys poios neos mphke
	strcpy(temp, "");
	strcpy(temp, sender);
	strcat(temp, " is online");
	for(int i=0; i< clientCount; i++){
		if(sock_ids[i] != currentSock){
			send(sock_ids[i], temp, 1024, 0);
		}	
	}
}

void client_handler(void *sock){
	Client newclient;		//apo to struct Client exoyme enan neo client
	newclient.sock=*(int*)sock;	//aparaithto to cast gt to sock einai kanonika int alla den mporoyme na to perasoyme sketo int epeidh theloyme kai thn syndesh
	recvfrom(newclient.sock,newclient.name,50,0,NULL,NULL); //pairnei to onoma toy client
	printf("%s is online\n",newclient.name);
	someone_is_online(newclient.sock, newclient.name);	//enhmerwnei toys allous oti kapoios mphke
	while(strcmp(msg,"exit") !=0){
			recvfrom(newclient.sock,msg,1024,0,NULL,NULL);	//pairnei to msg
			if(strcmp(msg,"exit")!=0){			//an einai exit tote termatizei
				printf("%s send: %s\n",newclient.name,msg);	//alliws to kanei print ston server kai to stelnei se olous
				//send to all the rest
				send_to_all(newclient.sock, msg, newclient.name);
			}
			else{
				printf("%s Left the room\n",newclient.name);
				clientCount--;		//metritis gia toys posous clients eksiphretei
			}
		}
}

int main(int argc,char *argv[]){

	int psock,sock,ret,i; 		//SOS MATHETE APO TIS DIAFANEIES TI KANOUN OI PARAKATW DHLVSEIS GIA TA SOCKETS TCP/IP
	struct sockaddr_in addr; //internet socket
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);//all interfaces
	addr.sin_port=htons(1111);
	psock=socket(AF_INET,SOCK_STREAM,0);
	bind(psock,(struct sockaddr *)&addr,sizeof(addr));	//kanei bind to passive socket me tis parapanw dhlwseis
	listen(psock,10);
	pthread_t id;		//dhlwsh id toy thread
	printf("\nwaiting for new connection...\n");

	while(1){

		sock = accept(psock,NULL,NULL);
		strcpy(msg,"");
		printf("connection established...\n");
		sock_ids[clientCount] = sock;		//apothikeyetai ston pinaka me ola ta sockets wste na mporoyme na steiloyme se olous mazika
		clientCount++;				//ayksanetai kata 1 o metritis gia toys clients
		ret=pthread_create(&id, NULL,(void*)client_handler,(void*)&sock); //kanei create to thread kai ektelnei ton client handler
	}	//SUPER SOOOOS MATHETE TI KANEI H KATHE PARAMETROS APO TIS DIAFANEIES

	close(sock);	//kleinei ta sockets, passive socket kai apeleytherwnei to thread
	printf("\nconnection closed\n");
	pthread_join(id,NULL);
	close(psock);
        
return 0;

}