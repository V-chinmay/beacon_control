#include"mm_socket.h"

void recv_till_eof(int sock,char* dest)
{
    char buff;
    
    while(buff!='\n')
    {
        if((recv(sock,&buff,1,MSG_DONTWAIT)>0))
        {
           *(dest++)=buff;
        }
    }
}

int main()
{
    int opt=1;
    int addr_len=sizeof(address_s);

    address_s.sin_family=AF_INET;
    address_s.sin_port=htons(PORT);
    int sockfd=socket(AF_INET,SOCK_STREAM,0);

    
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        return(0);
    } 

    if(bind(sockfd,(struct sockaddr *)&address_s,(socklen_t)addr_len))
    {
        printf("Failed to create socket!!\n");
        return(0);
    }
    else
    {
        printf("successfully created socket!!\n");
    }
    
    printf("listening!!\n");
 
    listen(sockfd,MAX_CON);

    int conn_fd=accept(sockfd,(struct sockaddr *)&address_s,(socklen_t*)&addr_len);
    
    printf("successfully made the connection!!\n");
    
    return(1);
}
