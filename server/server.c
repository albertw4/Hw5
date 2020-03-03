#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

typedef struct sockaddr SA;
int MAXLINE = 256;
char data[300][8][11];


int open_listenfd(char* portNum){
    int serverSocket;
    int optval = 1;
    struct addrinfo hints, *results, *p;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags |= AI_ADDRCONFIG;

    if((getaddrinfo(NULL, portNum, &hints, &results)) != 0){
        printf("Error Translating Socket\n");
        return -1;
    }
    printf("Socket Translated\n");

    for(p = results; p!= NULL; p = p->ai_next){
        serverSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(serverSocket < 0) continue;
        setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval , sizeof(int));
        if (bind(serverSocket, p->ai_addr, p->ai_addrlen) == 0) break; /* Success */
        close(serverSocket);
    }

    freeaddrinfo(results);

    if(p == NULL) return -1;
    if(listen(serverSocket, optval) < 0){
        close(serverSocket);
        return -1;
    }

    printf("Socket Created and Bounded Successful\n");

    return serverSocket;
}

void echo(int connfd){
    size_t n;
    char buf[MAXLINE];
    printf("server started");
    while((n = read(connfd, buf, MAXLINE)) != 0) {
        char * token = strtok(buf, " ");
        char str[2][11];
        char result[100];
        int i = 0;
        int length = 0;

        while(token != NULL){
           strcpy(str[i],token);
           i++;
           token = strtok(NULL, " ");
        }

        int k;
        for(k = 1; k < 270; k++){

          if(strncmp(str[0], data[k][1],10) == 0){
            if(strncmp(str[1], "type",4) == 0){
              strcpy(result, data[k][0]);
              printf("\n%s %s\n", str[0], "type");
            }else if(strncmp(str[1], "game_id",7) == 0){
              strcpy(result, data[k][1]);
              printf("\n%s %s\n", str[0], "game_id");
            }else if(strncmp(str[1], "home_team",9) == 0){
              strcpy(result, data[k][2]);
              printf("\n%s %s\n", str[0], "home_team");
            }else if(strncmp(str[1], "away_team",9) == 0){
              strcpy(result, data[k][3]);
              printf("\n%s %s\n", str[0], "away_team");
            }else if(strncmp(str[1], "week",4) == 0){
              strcpy(result, data[k][4]);
              printf("\n%s %s\n", str[0], "week");
            }else if(strncmp(str[1], "season",6) == 0){
              strcpy(result, data[k][5]);
              printf("\n%s %s\n", str[0], "season");
            }else if(strncmp(str[1], "home_score",10) == 0){
              strcpy(result, data[k][6]);
              printf("\n%s %s\n", str[0], "home_score");
            }else if(strncmp(str[1], "away_score",10) == 0){
              strcpy(result, data[k][7]);
              printf("\n%s %s\n", str[0], "away_score");
            }else{
              strcpy(result, "unknown");
            }

            break;
          }else{
            strcpy(result, "unknown");
          }
        }

        write(connfd, result, n);
    }
}

int main(int argc, char **argv)
{
    int listenfd, clientSocket;
    socklen_t clientLen;
    struct sockaddr_storage clientAddr;
    char* portNum = argv[2]; //"30000";
    char client_hostname[MAXLINE], client_port[MAXLINE];
    FILE *fp = fopen(argv[1], "r");

    int i = 0;
    char *pt;

    if(fp != NULL){
      char line[1024];

      while(fgets(line, sizeof line, fp) != NULL){
        int j = 0;
        pt = strtok (line,",");
        while (pt != NULL) {
            strcpy(data[i][j], pt);
            printf("|%s|", data[i][j]);
            pt = strtok (NULL, ",");
            j++;
        }
        i++;
      }
      fclose(fp);
    }else{
      printf("cant open file");
    }


    while(1){
        listenfd = open_listenfd(portNum); //change to argv[1] later
        clientLen = sizeof(struct sockaddr_storage);
        clientSocket = accept(listenfd, (SA *)&clientAddr, &clientLen);
        getnameinfo((SA *) &clientAddr, clientLen,client_hostname, MAXLINE, client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        echo(clientSocket);
        close(clientSocket);
    }

    return 0;
}
