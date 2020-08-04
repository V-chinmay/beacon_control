#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>

bool mm_open_port();
bool mm_get_last_locations(void*);
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

};

struct location_data
{
	struct device_location last_coordinates[6];
	bool raw_data_available;
	uint8_t reserved[5];
	uint8_t payload_size;//user can send custom payloads
	uint8_t payload_data[128];//must specify payload type

};

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
	struct list_of_devices l_o_d;
	printf("trying to get the device list!!\n");

	while(!mm_get_devices_list(&l_o_d))
	{
		sleep(1);
	}
	for(int i=0;i<l_o_d.number_of_devices;i++)
	{
		printf("address of beacons %d found is:: %d \n",i,l_o_d.devices[i].address);
	}
		
	
}
void get_telemetry(uint8_t beacon_addr)
{
	struct telemetry_info tele;
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
	struct location_data loc_data;
//printf("x:%d y:%d z:%d confidence:%d\n",loc_data.last_coordinates[1].x,loc_data.last_coordinates[1].y,loc_data.last_coordinates[1].z,loc_data.last_coordinates[1].confidence);
	for(int i=0;i<6;i++){
	loc_data.last_coordinates[i].x=0;
	loc_data.last_coordinates[i].y=0;
	loc_data.last_coordinates[i].z=0;
	loc_data.last_coordinates[i].confidence=0;
	}
	bool flag=mm_get_last_locations(&loc_data);
	/*while(!mm_get_last_locations(&loc_data))
	{
		sleep(1);
	}*/
	
	printf("workng::%d\n",flag);
	for(int i=0;i<6;i++)
	{
		printf("xxxxxasax:%d y:%d z:%d confidence:%d\n",loc_data.last_coordinates[i].x,loc_data.last_coordinates[i].y,loc_data.last_coordinates[i].z,loc_data.last_coordinates[i].confidence);
	

//		printf("address::%d\n",loc_data.last_coordinates[i].address);
	}

	
}


int main()

{
	struct list_of_devices l_o_d;
	open_serial_port();	
	get_devices_connected();
	get_telemetry(1);
	while(1)
	{
		get_latest_data();
		sleep(1);
	}

	mm_close_port();


	return(0);
}
