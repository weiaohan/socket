#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define PORT 1234
#define MAXDATASIZE 100

int main(int argc, char const *argv[])
{
    int fd, numbytes;
    char buf[MAXDATASIZE], buffto[MAXDATASIZE];
    struct hostent* he;
    struct sockaddr_in server;
    if (argc != 2) {
        printf("Usage: %s <IP address>\n", argv[0]);
        exit(-1);
    }
    if((he=gethostbyname(argv[1])) == NULL) {
        perror("Error:gethostbyname failed!");
        exit(1);
    }
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error: create socket failed!");
        exit(1);
    }
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr = *((struct in_addr*)he -> h_addr);
    server.sin_port = htons(PORT);
    if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1) {
        perror("Error: connet failed!");
        exit(1);
    }
    close(fd);
    return 0;
}