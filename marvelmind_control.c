#include <netinet/tcp.h>
#include "marvelmind_control.h"
void open_serial_port()
{
	printf("trying to open the serial port!!\n");
	while(!mm_open_port())
	{
		sleep(1);
	}
	printf("Successfully opened the serial port!!\n");


}

uint8_t get_devices_connected(int sockfd,bool send_flag)
{
	list_of_devices device_list;

    uint8_t hedgehog_address=0;
    char send_buff[50];
    char buff[50];
    printf("trying to get the device list!!\n");

	while(!mm_get_devices_list(&device_list))
	{
		sleep(1);
	}
	sprintf(send_buff,"#%d,",device_list.number_of_devices);

    for(int i=0;i<device_list.number_of_devices;i++)
	{
        sprintf(buff,"%d-%d,",device_list.devices[i].address,device_list.devices[i].sleep_flag);
        strcat(send_buff,buff);
		if(device_list.devices[i].sleep_flag)
        {
        printf("Beacon %d is sleeping \n",device_list.devices[i].address);    
        }
        else
        {
            printf("Beacon %d is awake \n",device_list.devices[i].address);
        }

        if(device_list.devices[i].device_type_id==31)
        {
            hedgehog_address=device_list.devices[i].address;
        }

	}
    sprintf(buff,"HED-%d#",hedgehog_address);
    strcat(send_buff,buff);
    //create a string containing device status over tcp
    printf("Got device list!! and is %s \n",send_buff);
    bool flag=1;
    if(send_flag)
    {
        setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    send(sockfd,send_buff,strlen(send_buff),MSG_DONTWAIT);
    }
    return(hedgehog_address);

}

void get_telemetry(uint8_t beacon_addr,int sockfd)
{
	telemetry_info tele;
	char send_buff[50];
    printf("getting telemetry data for %d\n",beacon_addr);
	while(!(mm_get_beacon_telemetry(beacon_addr,&tele)))
	{
		sleep(1);
	}
    sprintf(send_buff,"#%d,%d,%d,%.3f#",tele.time_from_reset,tele.radio_strength,tele.temperature,(tele.voltage/1000.0));
    /*printf("Time since reset::%d\n",tele.time_from_reset);
	printf("RSSI strength::%d\n",tele.radio_strength);
	printf("Temperature::%d\n",tele.temperature);
	printf("Battery voltage::%d\n",tele.voltage);
    */
    bool flag=1;

    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

    send(sockfd,send_buff,strlen(send_buff),MSG_DONTWAIT);

    printf("tele::%s\n",send_buff);
}


void get_latest_data(uint8_t hedgehog_address,int sockfd)
{
	location_packet loc_pac;
    int block_size=0;
	uint8_t buff[512];
    char send_buff[50];
    memset(buff,0,512);
    bool done_flag=0;
	mm_get_last_locations2(buff);
    for(int i=0;i<MAX_LOCATION_PACKET_SIZE;i++)
    {
        loc_pac.last_coordinates[i].address=*((uint8_t*)(buff+block_size));
        loc_pac.last_coordinates[i].head_index=*((uint8_t*)(buff+block_size+(1)));
        loc_pac.last_coordinates[i].x=*((uint32_t*)(buff+block_size+2));
        loc_pac.last_coordinates[i].y=*((uint32_t*)(buff+block_size+6));
        loc_pac.last_coordinates[i].z=*((uint32_t*)(buff+block_size+10));
        loc_pac.last_coordinates[i].status_flags=*((uint8_t*)(buff+block_size+14));
        loc_pac.last_coordinates[i].confidence=*((uint8_t*)(buff+block_size+15));
        loc_pac.last_coordinates[i].tbd[0]=*((uint8_t*)(buff+block_size+16));
        loc_pac.last_coordinates[i].tbd[1]=*((uint8_t*)(buff+block_size+17));
        loc_pac.last_coordinates[i].angle_data=*((uint16_t*)(buff+block_size+18));
        //loc_pac.last_coordinates[i].angle_ready=*((bool*)(buff+block_size+20));
        if(loc_pac.last_coordinates[i].address==hedgehog_address && done_flag==0)
        {
            bool flag=1;

            setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

            sprintf(send_buff,"#%d,%.3f,%.3f,%.3f,%d#",loc_pac.last_coordinates[i].address,mm_to_m(loc_pac.last_coordinates[i].x),mm_to_m(loc_pac.last_coordinates[i].y),mm_to_m(loc_pac.last_coordinates[i].z),loc_pac.last_coordinates[i].confidence);
            printf("coor::%s\n",send_buff);
            send(sockfd,send_buff,strlen(send_buff),MSG_DONTWAIT);
            done_flag=1;

        }
        block_size+=20;
    }

}

bool wake_device(uint8_t address,int sockfd)
{
    bool s_flag=mm_wake_device(address);
    char send_buff[10];
    sprintf(send_buff,"#%d#",s_flag);
    bool flag=1;

    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

    send(sockfd,send_buff,strlen(send_buff),MSG_DONTWAIT);

    return(s_flag);
}

bool sleep_device(uint8_t address,int sockfd)
{
    bool s_flag=mm_send_to_sleep_device(address);
    char send_buff[10];
    sprintf(send_buff,"#%d#",s_flag);
    bool flag=1;

    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

    send(sockfd,send_buff,strlen(send_buff),MSG_DONTWAIT);
    return(s_flag);
}

bool reset_device(uint8_t address,int sockfd)
{
    bool s_flag=mm_reset_device(address);
    char send_buff[10];
    bool flag=1;

    setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));


    sprintf(send_buff,"#%d#",s_flag);
    send(sockfd,send_buff,strlen(send_buff),MSG_DONTWAIT);

    return(s_flag);

}

