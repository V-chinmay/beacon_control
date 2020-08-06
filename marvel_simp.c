#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include<string.h>


#define MAX_NO_BEACONS 256
#define MAX_LOCATION_PACKET_SIZE 6
#define mm_to_m(in_mm) in_mm/1000.0


bool mm_open_port();
bool mm_reset_device (uint8_t address);
bool mm_get_last_locations2(void*);
bool mm_get_beacon_telemetry();
bool mm_get_devices_list (void *pdata);
bool mm_close_port();
bool mm_api_version(void *pdata);
bool mm_send_to_sleep_device(uint8_t addr);
bool mm_wake_device (uint8_t address);


struct api_v
{
	uint32_t vers;

};


struct device
{
	uint8_t address;
	bool same_address_flag;
	bool sleep_flag;
	uint8_t firmware_major_version;
	uint8_t firmware_minor_version;
	uint8_t firmware_second_minor_version;
	uint8_t device_type_id;
	uint8_t firware_options;
	uint8_t device_flags;//first bit shows whether device connected or is still trying to connect
};


struct list_of_devices
{
	uint8_t number_of_devices;
	struct device devices[MAX_NO_BEACONS];
    
};

struct telemetry_info
{
	uint32_t time_from_reset;
	int8_t radio_strength;
	int8_t temperature;
	uint16_t voltage;
	uint8_t reserved[16];

};

struct device_location
{
	uint8_t address;
	uint8_t head_index;
	int32_t x;
	int32_t y;
	int32_t z;
	uint8_t status_flags;
	uint8_t confidence;
	uint8_t tbd[2];
	uint16_t angle_data;
	bool angle_ready;
};

struct location_data
{
	struct device_location last_coordinates[MAX_LOCATION_PACKET_SIZE];
	//bool raw_data_available;
	//uint8_t reserved[5];
	//uint8_t payload_size;//user can send custom payloads
	//uint8_t payload_data[128];//must specify payload type

};

typedef struct location_data location_packet;
typedef struct device_location device_location;
typedef struct	telemetry_info telemetry_info;
typedef struct list_of_devices list_of_devices;
typedef struct device device;




void open_serial_port()
{
	printf("trying to open the serial port!!\n");
	while(!mm_open_port())
	{
		sleep(1);
	}
	printf("Successfully opened the serial port!!\n");


}

uint8_t get_devices_connected()
{
	list_of_devices device_list;
    
    uint8_t hedgehog_address=0;
	
    printf("trying to get the device list!!\n");

	while(!mm_get_devices_list(&device_list))
	{
		sleep(1);
	}

	for(int i=0;i<device_list.number_of_devices;i++)
	{
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
    //create a string containing device status over tcp
    printf("Got device list!!\n");

    return(hedgehog_address);

}

void get_telemetry(uint8_t beacon_addr)
{
	telemetry_info tele;
	printf("getting telemetry data for %d\n",beacon_addr);
	while(!(mm_get_beacon_telemetry(beacon_addr,&tele)))
	{
		sleep(1);
	}
	printf("Time since reset::%d\n",tele.time_from_reset);
	printf("RSSI strength::%d\n",tele.radio_strength);
	printf("Temperature::%d\n",tele.temperature);
	printf("Battery voltage::%d\n",tele.voltage);
}


void get_latest_data(uint8_t hedgehog_address)
{
	location_packet loc_pac;
    int block_size=0; 
	uint8_t buff[512];
    memset(buff,0,512);
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
        if(loc_pac.last_coordinates[i].address==hedgehog_address)
        {
             printf("hedgehog address::%d x::%.3f y::%.3f z::%.3f quality::%d \n",loc_pac.last_coordinates[i].address,mm_to_m(loc_pac.last_coordinates[i].x),mm_to_m(loc_pac.last_coordinates[i].y),mm_to_m(loc_pac.last_coordinates[i].z),loc_pac.last_coordinates[i].confidence);

        }
        block_size+=20;
    }

}

bool wake_device(uint8_t address)
{
    return(mm_wake_device(address));
}

bool sleep_device(uint8_t address)
{
    return(mm_send_to_sleep_device(address));
}

bool reset_device(uint8_t address)
{
    return(mm_reset_device(address));

}


int main()

{
    uint8_t hedgehog_address=0;
	char* input;
    uint8_t device_address=0;
    open_serial_port();
	hedgehog_address=get_devices_connected();

    while(1)
	{
        printf("enter command::\t");
        scanf("%s",input);
        printf("entered value is %s\n",input);

        if(!strcmp(input,"current"))
        {
            get_latest_data(hedgehog_address);    
        }

        if(!strcmp(input,"devlist"))
        {
            hedgehog_address=get_devices_connected();
        }

        if(!strcmp(input,"wake"))
        {
            printf("enter the address of beacon to wakeup:: ");
            scanf("%hhi",&device_address);
            if(wake_device(device_address))
            {
                printf("successfully woke up %hhi\n",device_address);
            }
            else
            {
                printf("failed wake up\n");
            }

        }
        if(!strcmp(input,"reset"))
        {
            printf("enter the address of beacon to reset:: ");
            scanf("%hhi",&device_address);
            if(reset_device(device_address))
            {
                printf("successfully sent %hhi to reset\n",device_address);
            }
            else
            {
                printf("failed send to reset\n");
            }
            
        }
        if(!strcmp(input,"sleep"))
        {
            printf("enter the address of beacon to sleep:: ");
            scanf("%hhi",&device_address);
            if(sleep_device(device_address))
            {
                printf("successfully sent %hhi to sleep\n",device_address);
            }
            else
            {
                printf("failed send to sleep\n");
            }
            
        }

        if(!strcmp(input,"status"))
        {
            printf("enter the address of beacon for status:: ");
            scanf("%hhi",&device_address);
            get_telemetry(device_address);
        }

        if(!strcmp(input,"exit"))
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
