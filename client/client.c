#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
typedef struct sockaddr SA;

// Opens a socket
int open_clientfd(char* hostname, char* portNum){
    int clientSocket;
    struct addrinfo hints, *results, *p;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags |= AI_ADDRCONFIG;

    if((getaddrinfo(hostname, portNum, &hints, &results)) != 0){
        printf("Error Translating Socket\n");
        return -1;
    }
    printf("Socket Translated\n");

    for(p = results; p!= NULL; p = p->ai_next){
        clientSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(clientSocket < 0) continue;
        if(connect(clientSocket, p->ai_addr, p->ai_addrlen) != -1) break;
        close(clientSocket);
    }

    freeaddrinfo(results);

    if(p == NULL) return -1;
    else{
        printf("Socket Created Successful\n");
        return clientSocket;
    }

}

int main(int argc, char **argv)
{
    struct sockaddr_in server_address;
    int clientfd;
    int MAXLINE = 256;
    char* command;
    char* hostName = "localhost"; //change to argv[1] later
    char* portNum = "30000";    //change to argv[2] later
    char input[MAXLINE];

    clientfd = open_clientfd(hostName,portNum);

    while(1){
        printf("\n> ");
        fgets(input,MAXLINE,stdin);
        char *newline = strchr(input, '\n');
        if(newline){*newline = 0;}
        command = strtok(input, " ");

        printf("Command is |%s|, length is %d\n", command, strlen(command));

        if(strcmp(command, "quit") == 0){
            break;
        }

        write(clientfd, command, strlen(command));
        read(clientfd, command, MAXLINE);
        fputs(command,stdout);

    }

    close(clientfd);

    return 0;
}
