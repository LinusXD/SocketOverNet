#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

static const char servIp[] = "127.0.0.1";
static const unsigned short port = 6666;
#define MAX_BUFF_SIZE 4096

int main(int ac, char** av)
{
    int sockFd;
    int len;
    struct sockaddr_in servAddr;
    char buffer[MAX_BUFF_SIZE] = {0};
    
    // Create a socket for client.
    sockFd = socket(AF_INET, SOCK_STREAM, 0);

    if(-1 == sockFd)
    {
        printf("Socket create failed!\n");
        exit(-1);
    }

    memset(&servAddr, 0, sizeof(struct sockaddr_in));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(servIp);
    servAddr.sin_port = htons(port);
    if(-1 == connect(sockFd, (struct sockaddr*)&servAddr, sizeof(struct sockaddr)))
    {
        printf("Connect failed!\n");
        exit(-1);
    }

    printf("Connected success.\n");
    len = recv(sockFd, buffer, MAX_BUFF_SIZE, 0);
    buffer[len] = '\0';
    printf("%s\n", buffer);

    while(1)
    {
        printf("Input string send to server, end with \"end\":\n");
        scanf("%s", buffer);
        if(!strcmp(buffer, "end"))
            break;
        len = send(sockFd, buffer, strlen(buffer), 0);
        len = recv(sockFd, buffer, MAX_BUFF_SIZE, 0);
        buffer[len] = '\0';
        printf("received: %s\n", buffer);

    }

    close(sockFd);

    return 0;
}

