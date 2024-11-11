#ifndef CONTROL_H
#define CONTROL_H
#include "mbed.h"
#include <cstdint>

/**
* @file control.h
*
* @brief Control thread definition module.
*
* @author Diego Aceituno Seoane
*
* Module that defines the required elements to communicate the threads and relevant information for signals.
*
*/

#define QUEUE_SIZE 10

#define ACCELEROMETER_SIGNAL 1U
#define GPS_SIGNAL           2U
//#define BRIGHTNESS_SIGNAL    4U No thread
#define TEMP_HUM_SIGNAL      8U
#define COLOR_SIGNAL         16U
//#define MOISTURE_SIGNAL      32U

/**
* @brief Enumeration for the entering msg to the main thread.
*/
typedef enum{
    ACCELEROMETER,
    GPS,
    TEMP_HUM,
    COLOR
}msg_type;


/**
* @brief Structure for the payload of the accelerometer msg.
*/
typedef struct{
    float x_acc;
    float y_acc;
    float z_acc;
}accelerometer_msg_t;

/**
* @brief Structure for the payload of the gps msg
*/
typedef struct{
    uint8_t sats;
    uint8_t time_h;
    uint8_t time_m;
    uint8_t time_s;
    uint8_t time_day;
    uint8_t time_month;
    uint16_t time_year;
    uint16_t altitude;
    char altitude_c;
    char lat_n;
    char lng_w;
    float lat;
    float lng;
}gps_msg_t;

/**
* @brief Structure for the payload of the temperature and humidity msg.
*/
typedef struct{
    float temp;
    float hum;
}temp_hum_msg_t;

/**
* @brief Structure for the payload of the color sensor msg.
*/
typedef struct{
    uint16_t clear;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
}color_msg_t;

/**
* @brief Structure for the msg entering the main thread.
*/
typedef struct{
    msg_type type;
    union{
        accelerometer_msg_t accelerometer_msg;
        gps_msg_t           gps_msg;
        temp_hum_msg_t      temp_hum_msg;
        color_msg_t         color_msg;
    };
}ctrl_msg;


extern Queue<ctrl_msg, QUEUE_SIZE> ctrl_in_queue;

#endif