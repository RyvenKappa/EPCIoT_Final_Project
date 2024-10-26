#ifndef BRIGHTNESS_SENSOR_H
#define BRIGHTNESS_SENSOR_H
#include "mbed.h"
/**
* @file brightness_sensor.h
*
* @brief Modulo para controlar el sensor de brillo.
*
* @author Diego Aceituno Seoane
*
* Controls the inicialization of the photo-transistor for the brightness.
* Controls of the measurements with a separate thread.
* This module is implemented taking into account that the voltage in the collector is 3.3 Volts.
* 
*/

/**
* @brief Measurement thread for the brightness sensor.
*/
extern Thread brightness_thread;

/**
* @brief Inicialization of the brightness_sensor_thread.
*/
extern void brightness_sensor_init();
#endif