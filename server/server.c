#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

typedef struct sockaddr SA;

int main()
{
    char server_message[250] = "Connected to Server BOI!";

    int server_socket;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //get server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(30000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //bind socket to our ip and port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    listen(server_socket, 3);

    int client_socket;

    accept()


    return 0;
}
