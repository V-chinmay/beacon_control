#include "marvelmind_control.h"
#include "mm_socket.h"
#include<pthread.h>

int exitFlag=0;

struct stream_info
{
    uint8_t hedgehog_address;
    int sockfd;
};



void stream_loc(void* info)
{
    int val=((struct stream_info*)info)->sockfd;
    while(exitFlag==0)
    {
        get_latest_data(((struct stream_info*)info)->hedgehog_address,val);
    }
}

int main()
{
    uint8_t hedgehog_address=4;
    char buff[30];
    char** input=NULL;
    uint8_t device_address=0;
    open_serial_port();
    pthread_t thread_id;
    struct stream_info info;

    info.hedgehog_address=4;
    int conn1=start_connection(LOC_PORT,LOC_CON);
    info.sockfd=conn1;
    printf("Hedgehog_address::%d\n",hedgehog_address);
    int conn=start_connection(REM_PORT,REM_CON);
    
 
    while(1)
	{
        if(!recv_till_eof(conn,&buff[0]))
        {
            int temp =conn;

            printf("device disconnected at PORT::%d!!\n",REM_PORT);
            
            if(close(conn)>=0){printf("closed connection successfully\n");}else
            {
                printf("failed to close connection\n!!");
            }
            
            conn=start_connection(REM_PORT,REM_CON);
            printf("fd is %d \n",conn);
            if(temp==conn)
            {
                printf("fd same!!\n");
            }
        }

        printf("received::%s\n",buff);
        input=split(buff,'-');

        if(!strcmp(*input,"CURRENT"))
        {
            exitFlag=0;
	        pthread_create(&thread_id,NULL,(void*)stream_loc,(void*)&info);
            send(conn,"started",strlen("started"),MSG_DONTWAIT);
            //get_latest_data(hedgehog_address,conn);
            printf("Sent the latest coordinates!!\n");
        }

        if(!strcmp(*input,"DEVLIST"))
        {
            exitFlag=1;
            info.hedgehog_address=get_devices_connected(conn,1);
            //send(conn,"done\n",5,MSG_DONTWAIT);
            printf("sent the connected devices list\n");

        }
        if(!strcmp(*input,"ALLBATT"))
        {
            get_battery_all(conn);
            printf("sent the battery status of all devices\n");

        }

        if(!strcmp(*input,"WAKE"))
        {
            //wake command format :: WAKE-1 
            exitFlag=1;
            device_address=atoi(*(input+1));
            if(wake_device(device_address,conn))
            {
                printf("successfully woke up %hhi\n",device_address);
            }
            else
            {
                printf("failed wake up\n");
            }

        }
        if(!strcmp(*input,"RESET"))
        {
            //reset command format :: RESET-1 

            exitFlag=1;
            device_address=atoi(*(input+1));

            if(reset_device(device_address,conn))
            {
                printf("successfully sent %hhi to reset\n",device_address);
            }
            else
            {
                printf("failed send to reset\n");
            }
            
        }
        if(!strcmp(*input,"SLEEP"))
        {
            //sleep command format :: SLEEP-1 

            exitFlag=1;
            device_address=atoi(*(input+1));
            
            if(sleep_device(device_address,conn))
            {
                printf("successfully sent %hhi to sleep\n",device_address);
            }
            else
            {
                printf("failed send to sleep\n");
            }
            
        }

        if(!strcmp(*input,"STATUS"))
        {
            //status command format :: STATUS-1 

            exitFlag=1;
            device_address=atoi(*(input+1));
            get_telemetry(device_address,conn);
            printf("Sent status of the beacon at  %d\n",device_address);

            
        }

        if(!strcmp(*input,"EXIT"))
        {

            printf("exiting!!\n");
            mm_close_port();
            exitFlag=1;
            exit(1);
        }
	printf("done\n");
		//sleep(0.5);
	}
    
	
	mm_close_port();


	return(0);
}
