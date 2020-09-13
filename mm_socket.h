#include <netinet/in.h>
#include <sys/socket.h> 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <arpa/inet.h>


#define LOC_PORT 2800
#define REM_PORT 3000
#define MAX_CON 1
#define LOC_CON 1
#define REM_CON 0

struct sockaddr_in address_s;

int recv_till_eof(int sock,char* dest);
int start_connection(int,char);
char** split(char* input,char spl_ch);



