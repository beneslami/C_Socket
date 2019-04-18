#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <string.h>

int SocketCreate(void){
    int hSocket = socket(AF_INET,SOCK_STREAM,0);
    return hSocket;
}

int SocketConnect(int socket, char* ip, int portNum){
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portNum);
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    memset(&(serv_addr.sin_zero), 0, 8);
    return connect(socket, (struct sockaddr*) &serv_addr, sizeof(struct sockaddr));
}

int SocketSend(int socket,char* request, int requestLengh){
    struct timeval tv;
    tv.tv_sec = 20;
    tv.tv_usec = 0;
    if(setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv))){
        printf("timeOut...\n");
        return -1;
    }
    return send(socket, request, requestLengh,0);
}

int SocketReceive(int socket,char *response, int receiveSize){
    struct timeval tv;
    tv.tv_sec = 20;
    tv.tv_usec = 0;
    if(setsockopt(socket,SOL_SOCKET,SO_RCVTIMEO,(char *)&tv, sizeof(tv))){
        printf("timeOut...\n");
        return -1;
    }
    int returnValue = recv(socket, response, receiveSize, 0);
    if(returnValue < 0){
        perror("cannot receive from server");
        exit(1);
    }
    printf("Server:%s\n", response);
    return returnValue;
}

void SendingFile(int socketfd, char *bufferWrite){

    bufferWrite="this is test";
    printf("%s\n", bufferWrite);
    if(send(socketfd, bufferWrite, strlen(bufferWrite), 0) < 0){
        perror("no data sent");
        exit(1);
    }
}

int main(int argc, char** argv) {
    int socketfd, portNum;
    char bufferRead[512];
    char bufferWrite[512];
    char* ip = argv[1];
    char* port = argv[2];

    if(argc < 3){
        perror("invlaid inputs");
        exit(0);
    }
    socketfd = SocketCreate();
    if(socketfd == -1){
        printf("Client-socket() is not created...\n");
        return 1;
    }
    printf("Client-socket() sockfd is OK...\n");
    if(SocketConnect(socketfd, ip, atoi(port)) < 0){
        perror("connect failed.\n");
        return 1;
    }
    printf("Client-socket() is successfully connected with server\n");
    sleep(1);
    bzero(bufferWrite, sizeof(bufferWrite));
    SendingFile(socketfd, bufferWrite);
    bzero(bufferRead, sizeof(bufferRead));
    int readSize = SocketReceive(socketfd, bufferRead,8);

    close(socketfd);
    return 0;
}
