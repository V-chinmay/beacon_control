#include <netinet/in.h>
#include <sys/socket.h> 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <arpa/inet.h>


#define PORT 2800
#define MAX_CON 2


struct sockaddr_in address_s;

int recv_till_eof(int sock,char* dest);
int start_connection(int);
char** split(char* input,char spl_ch);



