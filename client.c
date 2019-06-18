#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFFSIZE 512

void DieWithUserMessage(const char *msg, const char *detail){
  printf("%s: %s\n", msg, detail);
  exit(1);
}
void DieWithSystemMessage(const char *msg){
  perror(msg);
  exit(1);
}

int main(int argc, char** argv) {
  if(argc > 3)
    DieWithUserMessage("Parameter(s)", "<Server Address> <server port>");

  char *servIP = argv[1]; //server ip address. dotted quad
  in_port_t servPort = atoi(argv[2]);
  char echoString[BUFFSIZE]; //string to echo

  int echoStringLen = strlen("this is test"); //determine input length
  strcpy(echoString,"this is test");
  echoString[echoStringLen] = '\0';

  printf("%s\t%d\n", echoString, echoStringLen);
  //Create a reliable, stream socket using TCP
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock < 0)
    DieWithSystemMessage("socket() failed");
  else{
    printf("socket() is created successfully\n");
  }

  //Consruct the server address structure
  struct sockaddr_in servAddr; //server Address
  memset(&servAddr, 0, sizeof(servAddr)); //zero out structure
  servAddr.sin_family = AF_INET;

  //convert Address
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if(rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if(rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");
  servAddr.sin_port = htons(servPort); //server port

  //Establish the connection to the echo Server
  if(connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("connect() failed");
  else{
    printf("Connection established successfully\n");
  }

  while(1){
    printf("Enter data for server: \n");
    fgets(echoString, BUFFSIZE - 1, stdin);
    if ((send(sock, echoString, strlen(echoString), 0))== -1) {
      DieWithSystemMessage("send() failed");
    }
    else{
      char buffer[BUFFSIZE];
      int num = recv(sock, buffer, sizeof(buffer),0);
      if ( num <= 0 ){
        DieWithSystemMessage("recv() failed");
      }
      buffer[num] = '\0';
      printf("Server: %s\n",buffer);
    }
  }
  printf("\n");
  close(sock);
  return 0;
}
