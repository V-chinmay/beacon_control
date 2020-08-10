#include"mm_socket.h"
#include"marvelmind_control.h"
char** split(char* input,char spl_ch)
{
    static char* split_d[10];
    uint8_t word_count=0,prev_i=0;
    uint8_t inp_len=strlen(input);

    split_d[0]=input;

    for(uint8_t i=0;i<inp_len;i++)
    {
        split_d[word_count]=(input+prev_i);

        if(input[i]==spl_ch)
        {
            input[i]='\0';
            prev_i=(i+1);
            word_count++;
        }
    }
    return(&split_d[0]);
}

int recv_till_eof(int sock,char* dest)
{
    char buff;
    char flag;

    memset(dest,'\0',30);
    buff='\0';

    while(buff!='\n')
    {
        flag=recv(sock,&buff,1,MSG_DONTWAIT);
        if(flag>0 && buff!='\n')
        {
           *dest=buff;
           dest++;
        }
        else{

            if(flag==0)
            {
                return(0);
            }
            }
    }
    return(1);
}

int start_connection(int port_number)
{
    int opt=1;
    int addr_len=sizeof(address_s);

    address_s.sin_family=AF_INET;
    address_s.sin_port=htons(port_number);
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
        printf("successfully created socket at %d!!\n",port_number);
    }

    printf("listening!!\n");

    listen(sockfd,MAX_CON);

    int conn_fd=accept(sockfd,(struct sockaddr *)&address_s,(socklen_t*)&addr_len);

    printf("successfully made the connection!!\n");

    if(port_number==2801)
    {
        printf("The connection is local!!\n");
        while(1)
        {
            get_latest_data(conn_fd,4);
            //printf("sending something\n");
        }
    }

    return(conn_fd);
}
