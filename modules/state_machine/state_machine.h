#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include "mbed.h"
#include <cstdint>
#include <stdint.h>

/**
* @file state_machine.h
*
* @brief State_Machine main module.
*
* @author Diego Aceituno Seoane
*
* Module that implements an state_machine to control the events of the system.
* Controls the funcionality changes between states.
*
*/

#define QUEUE_SIZE 32

/**
* @brief Enumeration for the entering msg to the main thread.
*/
typedef enum{
    ACCELEROMETER,
    GPS,
    BRIGHTNESS,
    TEMP_HUM,
    COLOR,
    MOISTURE
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
    int sats;
    float lat;
    bool lat_n;
    float lng;
    bool lng_w;
    long altitude;
    int time_h;
    int time_m;
    int time_s;
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
        float               brightness_msg;
        temp_hum_msg_t      temp_hum_msg;
        color_msg_t         color_msg;
        float               moisture_msg;
    };
}ctrl_msg;


extern Queue<ctrl_msg, QUEUE_SIZE> ctrl_in_queue;

/**
* Function to setup:
*   - Initial State of the system.
*   - Board Leds initial state.
*/
extern void state_machine_init();

/**
* Function to do one cycle of the state_machine.
*/
extern void state_machine_cycle();
#endif