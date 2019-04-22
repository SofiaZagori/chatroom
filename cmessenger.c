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

int sock = 0;		//to socket aytou to client
int terminate = 0; 	//metavlith flag gia na termatisei

void recvHandler() {	//synarthsh apo to thread wste opoiadhpote stigmh na dexetai  
    char msg[1024];
    while (1) {
        int receive = recv(sock, msg, 1024, 0);	//dexetai to mhnyma, to kanoyme etsi gt otan h metavlith pairnei thn timh 1 
	if (receive > 0) {			//tote doulevei swsta alliws an einai 0 tote apetyxe
            printf("%s\n", msg);
        }
		else{
			printf("error in recvHandler....\n");
            break;
        }
		
    }
}

void sendHandler() {	//synarthsh gia na steilei mhnyma
    char msg[1024];
    while (1) {
        fgets(msg,1024,stdin);		//xrhsimopoioyme genika panta fget, kai oxi gets gia ta strings gia na kathorizoyme to megethos toy buffer
		msg[strlen(msg)-1] = '\0';	//PANTA kanoyme afto gia na afairesoyme to enter \0
        send(sock, msg, strlen(msg) + 1, 0);
        if (strcmp(msg, "exit") == 0) {
			printf("Client logged out\n");
			send(sock, msg, strlen(msg) + 1, 0);
			terminate = 1;		//to flag pairnei timh 1 kai termatizei
			break;
        }
	}
}

int main(int argc,char *argv[]){
	int i;
	char msg[1024],name[50];
	struct sockaddr_in addr;	//pali dhlwsh sockets sas parakalw deite ta apo tis diafaneies kala kala 
					//kai tis synarthseis poy xrhsimopoiountai kai tis parametroys ths kathemias gt ta thelei analytika
	addr.sin_family=AF_INET;
	inet_aton("127.0.0.1",&addr.sin_addr); //localhost
	addr.sin_port=htons(1111);
	sock=socket(AF_INET,SOCK_STREAM,0); //stream socket
	connect(sock,(struct sockaddr *)&addr,sizeof(addr));
	printf("connected, pls give nickname\n");

	fgets(name,50,stdin);		//pairnei to onoma 
	name[strlen(name)-1]='\0';
	send(sock,name,strlen(name)+1,0);

	pthread_t sendThread;		//anoigei 2 threads ena gia na stelnei kai ena gia na dexetai 
    if (pthread_create(&sendThread, NULL, (void *) sendHandler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

    pthread_t recvThread;
    if (pthread_create(&recvThread, NULL, (void *) recvHandler, NULL) != 0) {
        printf ("Create pthread error!\n");
        exit(EXIT_FAILURE);
    }

	//perimenei mexri na steilei o client exit 
	while(1){
		if(terminate == 1){
			break;
		}
	}
	close(sock);

return 0;
}