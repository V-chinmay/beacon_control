#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>
//#include <time.h>
#include <unistd.h>
bool mm_open_port();
bool mm_get_last_locations2(void*);
bool mm_get_beacon_telemetry();
bool mm_get_devices_list (void *pdata);
bool mm_close_port();
//bool mm_open_port_by_name();
bool mm_api_version(void *pdata);
bool mm_send_to_sleep_device(uint8_t addr);

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
	uint8_t device_flags;
};


struct list_of_devices
{
	uint8_t number_of_devices;
	struct device devices[250];

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
	struct device_location last_coordinates[6];
	bool raw_data_available;
	uint8_t reserved[5];
	uint8_t payload_size;//user can send custom payloads
	uint8_t payload_data[128];//must specify payload type

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

void get_devices_connected()
{
	list_of_devices device_list;
	printf("trying to get the device list!!\n");

	while(!mm_get_devices_list(&device_list))
	{
		sleep(1);
	}
	for(int i=0;i<device_list.number_of_devices;i++)
	{
		printf("address of beacons %d found is:: %d \n",i,device_list.devices[i].address);
	}


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


void get_latest_data()
{
	location_packet loc_pac;
    int block_size=0; 
    printf("entered bra\n");
	uint8_t buff[512];
    printf("some functio issue");
	mm_get_last_locations2(buff);
    for(int i=0;i<6;i++)
    {
        printf("entered\n");
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

        printf("address::%d x::%d y::%d z::%d quality::%d \n",loc_pac.last_coordinates[i].address,loc_pac.last_coordinates[i].x,loc_pac.last_coordinates[i].y,loc_pac.last_coordinates[i].z,loc_pac.last_coordinates[i].confidence);
        block_size+=20;
    }

}


int main()

{
	open_serial_port();
	get_devices_connected();
	get_telemetry(1);
	while(1)
	{
		get_latest_data();
		
		/*flags=mmGetLastLocations2(&loc_dat);
		for(int i=0;i<6;i++)
		{
			printf("x:%d y::%d z::%d quality::%d\n",loc_dat.pos[i].x_mm,loc_dat.pos[i].y_mm,loc_dat.pos[i].z_mm,loc_dat.pos[i].quality);
		}
		*/
		sleep(0.5);
	}

	mm_close_port();


	return(0);
}
