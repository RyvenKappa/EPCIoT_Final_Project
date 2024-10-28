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

/**
* @brief Measurement thread for the color sensor.
*/
extern Thread color_thread;

/**
* @brief Inicialization of the color thread.
*/
extern void  color_sensor_init();

#endif