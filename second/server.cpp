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
#define MAXDATASIZE 100

int reverse(char* str) {
    int i = 0;
    int len = strlen(str);
    char tmp;
    int length = len/2;
    while(i != length) {
        tmp = str[i];
        str[i] = str[len - 1];
        str[len - 1] = tmp;
        i++;
        len--;
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    int listenfd, connectfd;
    int numbytes;
    char buf[MAXDATASIZE];
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
        if ((connectfd = accept(listenfd, (struct sockaddr*)&client, &clilen)) == -1) {
            perror("Error: Accept failed!");
        } 
        printf ("You got a connection from  %s, port=%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        while(1) {
            if ((numbytes = recv(connectfd, buf, MAXDATASIZE, 0)) == -1) {
            perror("recv error");
                } 
            if (numbytes) {
            buf[numbytes] = '\0';
            if ( buf[0] == 'q') {
                break;
            }
            reverse(buf);
            if(send(connectfd, buf, strlen(buf) + 1, 0) == -1) {
                perror("send error");
                exit(-1);
                }
            } 
        }
        close(connectfd);
    }
    return 0;
}
