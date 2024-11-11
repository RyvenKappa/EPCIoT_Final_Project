#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H
#include "mbed.h"
/**
* @file color_sensor.h
*
* @brief Color sensor control module.
*
* @author Diego Aceituno Seoane
*
* Controls a TCS3472 color sensor:
* - RGB and Clear Light sensing with IR Filter.
* - Low power states.
*
*/

#define COLOR_SENSOR_LED PA_9


#define COLOR_SENSOR_SLAVE_ADDRESS 0x29

#define COLOR_SENSOR_CMD_MSK 0x80

#define COLOR_SENSOR_TYPE_AUTO_INCREMENT 0x20 //Working interest for this project

#define COLOR_SENSOR_ENABLE 0x00
#define COLOR_SENSOR_ATIME 0x01
#define COLOR_SENSOR_WTIME 0x03
#define COLOR_SENSOR_AILTL 0x04
#define COLOR_SENSOR_AILTH 0x05
#define COLOR_SENSOR_AIHTL 0x06
#define COLOR_SENSOR_AIHTH 0x07
#define COLOR_SENSOR_PERS 0x0C
#define COLOR_SENSOR_CONFIG 0x0D
#define COLOR_SENSOR_CONTROL 0x0F
#define COLOR_SENSOR_ID 0x12
#define COLOR_SENSOR_STATUS 0x13
#define COLOR_SENSOR_CDATAL 0x14
#define COLOR_SENSOR_CDATAH 0x15
#define COLOR_SENSOR_RDATAL 0x16
#define COLOR_SENSOR_RDATAH 0x17
#define COLOR_SENSOR_GDATAL 0x18
#define COLOR_SENSOR_GDATAH 0x19
#define COLOR_SENSOR_BDATAL 0x1A
#define COLOR_SENSOR_BDATAH 0x1B

/**
* @brief Reading of the color data.
*/
extern void  color_sensor_read();

/**
* @brief Inicialization of the color sensor.
*/
extern void  color_sensor_init();

#endif