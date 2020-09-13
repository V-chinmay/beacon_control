#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include<string.h>
#include"mm_socket.h"

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
void open_serial_port();
uint8_t get_devices_connected(int,bool);
void get_telemetry(uint8_t beacon_addr,int);
void get_latest_data(uint8_t hedgehog_address,int);
bool wake_device(uint8_t address,int);
bool sleep_device(uint8_t address,int);
bool reset_device(uint8_t address,int);
void get_battery_all(int);


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
