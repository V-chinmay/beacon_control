#include "marvelmind_control.h"
#include "mm_socket.h"




int main()
{
    uint8_t hedgehog_address=0;
	char buff[30];
    char** input=NULL;
    uint8_t device_address=0;
    open_serial_port();
	
    int conn=start_connection();
    
    hedgehog_address=get_devices_connected(conn,0);
    
    
 
    while(1)
	{
        if(!recv_till_eof(conn,&buff[0]))
        {
            printf("device disconnected!!\n");
            conn=start_connection();
        }

        printf("received %s\n",buff);
        input=split(buff,'-');

        if(!strcmp(*input,"current"))
        {
            get_latest_data(hedgehog_address,conn);
            printf("Sent the latest coordinates!!\n");
        }

        if(!strcmp(*input,"devlist"))
        {
            hedgehog_address=get_devices_connected(conn,1);
            //send(conn,"done\n",5,MSG_DONTWAIT);
            printf("sent the connected devices list\n");

        }

        if(!strcmp(*input,"wake"))
        {
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
        if(!strcmp(*input,"reset"))
        {
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
        if(!strcmp(*input,"sleep"))
        {
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

        if(!strcmp(*input,"status"))
        {
            device_address=atoi(*(input+1));
            get_telemetry(device_address,conn);
            printf("Sent status of the beacon at  %d\n",device_address);

            
        }

        if(!strcmp(*input,"exit"))
        {
            printf("exiting!!\n");
            mm_close_port();
            exit(1);
        }
		//sleep(0.5);
	}
    

	mm_close_port();


	return(0);
}
