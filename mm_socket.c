#include"mm_socket.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <arpa/inet.h>


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
    char client_inp[30];
    int opt=1;
    char server_buff[30]="hello from server";
    int addr_len=sizeof(address_s);

    address_s.sin_family=AF_INET;

    //address_s.sin_addr.s_addr=INADDR_ANY;
    address_s.sin_port=htons(1500);
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    /*if(inet_pton(AF_INET, "192.168.0.18", &address_s.sin_addr)<0)
    {
        printf("invalid address!!\n");
    }*/

    if(bind(sockfd,(struct sockaddr *)&address_s,(socklen_t)addr_len))
    {
        printf("Failed to create socket!!\n");
    }
    else
    {
        printf("successfully created socket!!\n");
    }
    
    printf("listening!!\n");
    listen(sockfd,2);

    int conn_fd=accept(sockfd,(struct sockaddr *)&address_s,(socklen_t*)&addr_len);
    printf("recived connection!!\n");
    recv_till_eof(conn_fd,&client_inp[0]);
    printf("client sent::%s\n",client_inp);

    if(!send(conn_fd,&server_buff,strlen(server_buff),MSG_DONTWAIT))
    {
        printf("no data sent!!");
    }
    else
    {
        printf("successfully sent data to client!!\n");
    }
    close(conn_fd);
    close(sockfd);
    return(1);



}
