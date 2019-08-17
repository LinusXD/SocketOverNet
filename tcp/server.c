#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

static const unsigned short port = 6666;
#define MAX_BUFF_SIZE 4096


int main(int ac, char** av)
{
    int serSockFd, cliSockFd;
    int len;
    struct sockaddr_in localAddr;
    struct sockaddr_in remoteAddr;

    int sinSize;
    char buffer[MAX_BUFF_SIZE] = {0};

    serSockFd = socket(AF_INET, SOCK_STREAM, 0);
    if(serSockFd == -1)
    {
        printf("Create socket failed!\n");
        exit(-1);
    }

    memset(&localAddr, 0, sizeof(struct sockaddr_in));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(port);

    if(-1 == bind(serSockFd, (struct sockaddr*)&localAddr, sizeof(struct sockaddr)))
    {
        printf("Bind error!\n");
    }

    listen(serSockFd, 5);
    sinSize = sizeof(struct sockaddr_in);

    // Waiting for clients' accepting.
    cliSockFd = accept(serSockFd, (struct sockaddr*)&remoteAddr, &sinSize);
    if(cliSockFd == -1)
    {
        printf("Accept error!\n");
        exit(-1);
    }

    printf("Accepted client: %s\n", inet_ntoa(remoteAddr.sin_addr));
    len = send(cliSockFd, "!!! WELCOME !!!\n", 21, 0);

    while((len = recv(cliSockFd, buffer, MAX_BUFF_SIZE, 0)) > 0)
    {
        buffer[len] = '\0';
        printf("%s\n", buffer);
        if(-1 == send(cliSockFd, buffer, len, 0))
        {
            printf("Send error!\n");
            break;;
        }
    }

    close(cliSockFd);
    close(serSockFd);


    return 0;
}