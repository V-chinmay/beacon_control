#include <netinet/in.h>
#include <sys/socket.h> 

#define PORT 2800
#define IP_addr "192.168.0.10"

int sockfd=0;

struct sockaddr_in address_s;

