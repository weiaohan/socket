#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread/pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 1234
#define BACKLOG 5
#define MAXDATASIZE 100

pthread_key_t key;

int reverse(char* str);
void* start_routine(void* arg);

struct ARG {
    int connectfd;
    struct sockaddr_in client;
};

int main(int argc, char const *argv[])
{
    int listenfd, connectfd;
    ARG* arg;
    struct sockaddr_in server, client;
    socklen_t clilen;
    pthread_t tid;

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
        arg = new ARG;
        arg -> connectfd = connectfd;
        memcpy((void*)&arg -> client, &client, sizeof(client));
        if(pthread_create(&tid, NULL, start_routine, (void*)arg)) {
            perror("Error: thread create failed!");
        }
    }
    close(listenfd);
    return 0;
}

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

void* start_routine(void* arg) {
    struct ARG *info;
    int index = 0;
    int numbytes;
    char buf[MAXDATASIZE];
    char sum[1000];
    info = (struct ARG*)arg;
    pthread_key_create(&key, NULL);
    printf ("[>]You got a connection from  %s\n", inet_ntoa(info -> client.sin_addr));
    while(1) {
            if ((numbytes = recv(info->connectfd, buf, MAXDATASIZE, 0)) == -1) {
                perror("recv error");
                break;
            }
            pthread_setspecific(key, (void*)sum);
            if (buf[0] == 'q' && strlen(buf) == 1) {
                printf("[!]pthread exit\n");
                sum[index + 1] = '\0';
                printf("[>]recv all: %s\n", pthread_getspecific(key));
                close(info->connectfd);
                break;
            } 
            for (int i = 0; i < strlen(buf); i ++) {
                sum[index++] = buf[i];
            }
            if (numbytes) {
            buf[numbytes] = '\0';
            reverse(buf);
            if(send(info->connectfd, buf, strlen(buf) + 1, 0) == -1) {
                perror("send error");
                exit(-1);
                }
            } 
        }
    free(arg);
    pthread_exit(NULL);
}