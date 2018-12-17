#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 1234
#define BACKLOG 5

int main(int argc, char const *argv[])
{
    int listenfd, connectfd;
    int sin_size;
    struct sockaddr_in server, client;
    socklen_t clilen;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1 ) {
        perror("Error: Create listen socket failed!");
        exit(-1);
    }
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);
    if (bind(listenfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("Error: Bind failed!");
        exit(-1);
    }
    if (listen(listenfd, BACKLOG) == -1) {
        perror("Error: Listen failed");
        exit(-1);
    }
    while(1) {
        clilen = sizeof(client);
        if ((connectfd = accept(listenfd, (struct sockaddr*)&client, &clilen) == -1)) {
            perror("Error: Accept failed!");
        } 
        printf ("You got a connection from %s\n", inet_ntoa(client.sin_addr));
        close(connectfd);
    }
    return 0;
}
