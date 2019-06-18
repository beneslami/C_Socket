#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <zconf.h>
#include <arpa/inet.h>

#define MAX_PENDING 5

int SocketCreate(void){
    int hSocket = socket(AF_INET,SOCK_STREAM,0);
    return hSocket;
}

int BindCreateSocket(int socket, int portNum){

    struct sockaddr_in serv_addr, cli_addr;  //structure containing internet address which is defined in netinet/in.h
    bzero((char *) &serv_addr, sizeof(serv_addr));  //memset(&serv_addr, 0, sizeof(serv_addr))
    serv_addr.sin_port = htons(portNum);
    serv_addr.sin_addr.s_addr = INADDR_ANY; // listen to any address
    return bind(socket, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
}

int serv_it(int socket, char *clientName, char *buffer){
    bzero(buffer, 1500);
    int numByte = recv(socket, buffer, 1500, 0);
    buffer[numByte] = '\0';
    if( numByte < 0){
      perror("cannot receive from client");
      return -1;
    }
    if(numByte == 0){
      printf("connection closed with client %s\n", clientName);
      return 0;
    }
    printf("from %s: %s\n", clientName, buffer);
    if (send(socket, "done", strlen("done"), 0) < 0){
      perror("cannot send to the destination");
    }
    if(strcmp(buffer,"quit") == 0){
        return 0;
    }
    return 1;
}

int main(int argc, char **argv){
    int socketfd; //store the value returned by socket sysCall
    int clientLength; // stores the size of the address of the client
    int newsocketfd;
    char bufferRead[256]; //server reads characters from the socket connections into this buffer
    struct sockaddr_in server, client;

    if (argc != 2) {
        perror("ERROR, port is not initialized");
        exit(1);
    }

    in_port_t port = atoi(argv[1]);
    // socket creation
    socketfd = SocketCreate();
    if(socketfd == -1){
        perror("Server-socket() error!");
        return 1;
    }
    printf("Server-socket() created\n");

    //binding process
    if(BindCreateSocket(socketfd, port) < 0){
        perror("Server-socket-bind() failed");
        return 1;
    }
    printf("bind done\n");

    //listen
    if(listen(socketfd, MAX_PENDING) == -1){
      perror("Server-socket-listen() failed");
      exit(1);
    }

    while(1){ //for(;;)
        printf("Waiting for incoming connections...\n");
        clientLength = sizeof(struct sockaddr_in);
        newsocketfd = accept(socketfd, (struct sockaddr*) &client, &clientLength);
        if(newsocketfd < 0){
            perror("accept failed");
            return 1;
        }
        char clntName[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr.s_addr, clntName, sizeof(clntName));
        printf("connection established\n");
        while(1){
            int result = serv_it(newsocketfd, clntName, bufferRead);
            if(result == -1 || result == 0){
              break;
            }
        }
        close(newsocketfd);
    }
    close(socketfd);
    return 0;
}
