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


#define defined_length_of_given_name 31
#define defined_length_of_given_message 100
#define defined_length_of_sent_message 200


// Global variables
int server_sockfd = 0, client_sockfd = 0;    // einai ta "psocket" mas


typedef struct ClientNode {
    int data;
    struct ClientNode* proigoumenos;
    struct ClientNode* endiamesos;
    char ip[16];  // server ip
    char name[20];
} ClientList;

ClientList *arxikos, *paron;

ClientList *newNode(int sockfd, char* ip) {  // DN MPOROUME NA AFAIRESOUME TIN IP GT VGAZEI PROBLEM
    ClientList *np = (ClientList *)malloc( sizeof(ClientList) );
    np->data = sockfd;  //to data einai to fd toy socket
    np->proigoumenos = NULL;
    np->endiamesos = NULL;
    strncpy(np->ip, ip, 16);
    strncpy(np->name, "NULL", 10); copies up to n characters from the string pointed to, by src to dest. To teleytaio einai to megethos twn stoixeiwn poy antigrafei.
    return np;
}


void str_trim_lf (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

void str_overwrite_stdout() {
    printf("\r%s", "> ");
    fflush(stdout);
}

void send_to_all_clients(ClientList *np, char tmp_buffer[]) {
    ClientList *tmp = arxikos->endiamesos;
    while (tmp != NULL) {
        if (np->data != tmp->data) { // all clients except itself.
            printf("Send to sockfd %d: \"%s\" \n", tmp->data, tmp_buffer);  //LEEI OTI O BUFFER TO STELNEI SE ALLOUS
            send(tmp->data, tmp_buffer, defined_length_of_sent_message, 0);
        }
        tmp = tmp->endiamesos;
    }
}

void client_handler(void *p_client) {
    int exodos = 0;
    char given_name[defined_length_of_given_name] = {};
    char recv_buffer[defined_length_of_given_message] = {};
    char send_buffer[defined_length_of_sent_message] = {};
    ClientList *np = (ClientList *)p_client;

    // Naming
    if (recv(np->data, given_name, defined_length_of_given_name, 0) <= 0 || strlen(given_name) < 2 || strlen(given_name) >= defined_length_of_given_name-1) {
        printf("No name was given.\n");
        exodos = 1;
    } else {
        strncpy(np->name, given_name, defined_length_of_given_name); // kanei copy to given_name sto "name" tou client
        printf("%s joined the chatroom.\n", np->name);
        sprintf(send_buffer, "%s joined the chatroom.", np->name);
        send_to_all_clients(np, send_buffer);
    }

    // Conversation
    while (1) {
        if (exodos) {
            break;
        }
        int receive = recv(np->data, recv_buffer, defined_length_of_given_message, 0);
        if (receive > 0) {
            if (strlen(recv_buffer) == 0) {
                continue;
            }
            sprintf(send_buffer, "%s：%s", np->name, recv_buffer);
        } else if (receive == 0 || strcmp(recv_buffer, "exit") == 0) {
            printf("%s left the chatroom.\n", np->name);
            sprintf(send_buffer, "%s left the chatroom.", np->name);
            exodos = 1;
        } else {
            printf("Fatal Error: -1\n");
            exodos = 1;
        }
        send_to_all_clients(np, send_buffer); //mipos na min to exoume se synartisi???
    }

    // Remove Node
    close(np->data);
    if (np == paron) { // remove an edge node
        paron = np->proigoumenos;
        paron->endiamesos = NULL;
    } else { // remove a middle node
        np->proigoumenos->endiamesos = np->endiamesos;
        np->endiamesos->proigoumenos = np->proigoumenos;
    }
    free(np);
}




int main()
{
    // Create socket
    server_sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (server_sockfd == -1) {
        printf("Fail to create a socket.");
        exit(EXIT_FAILURE);
    }

    // Socket information
    struct sockaddr_in addr;

    int addrlen = sizeof(struct sockaddr_in);

   
    memset(&addr, 0, addrlen);


//server info
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8888);


    // Bind and Listen
    bind(server_sockfd, (struct sockaddr *)&addr, addrlen); 
    listen(server_sockfd, 10); // The backlog, - the second number - defines the maximum length to which the queue of pending connections for sockfd may grow


    // Initial linked list for clients
    arxikos = newNode(server_sockfd, inet_ntoa(addr.sin_addr));
    paron = arxikos;

    while (1) {
        client_sockfd = accept(server_sockfd, (struct sockaddr*) &addr, (socklen_t*) &addrlen);


        // Append linked list for clients
        ClientList *c = newNode(client_sockfd, inet_ntoa(addr.sin_addr));
        c->proigoumenos = paron;
        paron->endiamesos = c;
        paron = c;

        pthread_t id;
        if (pthread_create(&id, NULL, (void *)client_handler, (void *)c) != 0) {
            perror("Create pthread error!\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

