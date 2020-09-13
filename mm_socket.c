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
        flag=recv(sock,&buff,1,MSG_WAITALL);
        if(flag>0 && buff!='\n')
        {
           *dest=buff;
           dest++;
        }
        else
	{
            if(flag==0)
            {
                return(0);
            }
        }
	sleep(0.5);
    }
    return(1);
}

int start_connection(int port_number,char local_flag)
{
    int opt=1;
    
    address_s.sin_family=AF_INET;
    address_s.sin_port=htons(port_number);

    if(local_flag)
    {
        printf("Connection is local\n");
	    inet_pton(AF_INET, "127.0.0.1", &address_s.sin_addr);
    	address_s.sin_addr.s_addr=htonl(INADDR_ANY);
    }
    else
    {
        printf("Connection is Remote\n");
    	address_s.sin_addr.s_addr=htonl(INADDR_ANY);
    }

    int sockfd=socket(AF_INET,SOCK_STREAM,0);


    /*if(inet_pton(AF_INET, "192.168.0.11", &address_s.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    } 
	*/
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        return(0);
    }
    int addr_len=sizeof(address_s);

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

    
    if(listen(sockfd,MAX_CON)>0)
    {
        printf("is listening\n");
    }
    printf("finished listening!!\n");
    
    int conn_fd=accept(sockfd,(struct sockaddr *)&address_s,(socklen_t*)&addr_len);

    printf("successfully made the connection!!\n");


    return(conn_fd);
}
