#include "gps.h"
#include "../state_machine/control.h"
#include <cstdint>
Thread gps_thread(osPriorityNormal,1024,nullptr,"GPSThread");

static ctrl_msg ctrl_msg_t;
static char buffer[83];
static BufferedSerial serial(NC,GPS_RX,9600);
static bool wait_gps_msg = 0;


int buffer_index = 0;
char c;

static bool found_gps_msg = false;

static float time_stamp = 240405.000;
static float latitude = 0.0;
static char latitude_c = 'N';
static float longitude = 0.0;
static char longitude_c = 'W';
static uint8_t quality = 0;
static uint8_t sats = 0;
static float altitude = 0.0;
static char altitude_c = 'm';
static float hdop = 0.0;

/**
* Static function to analyze the gps msg and find the specific one we need to fill a msg.
*/
//TODO crashea, mandarlo al main para no perder tanta memoria en esto
static void parse_gps_msg(){
    if (strncmp(buffer, "$GPGGA",6)==0){
        sscanf(buffer, "$GPGGA,%f,%f,%c,%f,%c,%hhu,%hhu,%f,%f,%c", &time_stamp,&latitude,&latitude_c,&longitude,&longitude_c,&quality,&sats,&hdop,&altitude,&altitude_c);
        ctrl_msg_t.gps_msg.time_h = (uint8_t)(time_stamp/10000);
        ctrl_msg_t.gps_msg.time_m = (uint8_t)(((uint16_t)time_stamp/100) % 100);
        ctrl_msg_t.gps_msg.time_s = (uint8_t)((uint16_t)time_stamp%100);
        ctrl_msg_t.gps_msg.altitude = altitude;
        ctrl_msg_t.gps_msg.altitude_c = altitude_c;
        ctrl_msg_t.gps_msg.lat = latitude/100.0;
        ctrl_msg_t.gps_msg.lat_n = latitude_c;
        ctrl_msg_t.gps_msg.lng = longitude/100.0;
        ctrl_msg_t.gps_msg.lng_w = longitude_c;
        ctrl_msg_t.gps_msg.sats = sats;
        found_gps_msg = true;
    }
}

/**
* Static function to read the gps data
*/
static void gps_read_data(){
    ctrl_msg_t.type = GPS;
    while (true) {
        ThisThread::flags_wait_all(GPS_SIGNAL);
        found_gps_msg = false;
        serial.enable_input(true);
        while (!found_gps_msg) {
            serial.read(&c,1);
            if (c == '\n') {
                buffer[buffer_index] = '\0';
                buffer_index = 0;
                parse_gps_msg();
            }else{
                buffer[buffer_index++] = c;
            }
        }
        serial.enable_input(false);
        ctrl_in_queue.try_put_for(Kernel::wait_for_u32_forever, &ctrl_msg_t);
    }
}

void gps_init(){
    serial.enable_input(false);
    gps_thread.start(&gps_read_data);
}