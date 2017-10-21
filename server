#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <zconf.h>

int SocketCreate(void){
    int hSocket = socket(AF_INET,SOCK_STREAM,0);
    return hSocket;
}

int BindCreateSocket(int socket){
    int portNum = 3128;
    struct sockaddr_in serv_addr, cli_addr;  //structure containing internet address which is defined in netinet/in.h
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_port = htons(portNum);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    return bind(socket, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
}

void serv_it(int socket,char *buffer){
    FILE *fd = fopen("output.txt","w+");
    while (1){
        bzero(buffer,1500);
        recv(socket,buffer,1500,0);
        if(strcmp(buffer,"quit") == 0) break;
        fwrite(buffer,1,1500,fd);
        printf("%s",buffer);
    }
    fclose(fd);
}

int main(int argc, char *argv[]){
    int socketfd; //store the value returned by socket sysCall
    int clientLength; // stores the size of the address of the client
    int newsocketfd;
    char bufferRead[256]; //server reads characters from the socket connections into this buffer
    struct sockaddr_in server, client;

    if (argc < 2) {
        fprintf(stderr,"ERROR, port is not initialized");
        exit(1);
    }

    // socket creation
    socketfd = SocketCreate();
    if(socketfd == -1){
        printf("socket is not created...");
        return 1;
    }
    printf("socket created\n");

    //binding process
    if(BindCreateSocket(socketfd) < 0){
        perror("bind failed");
        return 1;
    }
    printf("bind done\n");

    //listen
    listen(socketfd,5);

    while(1){
        printf("Waiting for incoming connections...\n");
        clientLength = sizeof(struct sockaddr_in);
        newsocketfd = accept(socketfd, (struct sockaddr*) &client, (socklen_t *) &clientLength);
        if(newsocketfd < 0){
            perror("accept failed");
            return 1;
        }
        printf("connection established\n");

        serv_it(newsocketfd,bufferRead);
        send(newsocketfd,"done",strlen("done"),0);
        close(newsocketfd);
    }
    close(socketfd);
    return 0;
}
