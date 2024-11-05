#ifndef BRIGHTNESS_SENSOR_H
#define BRIGHTNESS_SENSOR_H
#include "mbed.h"
/**
* @file brightness_sensor.h
*
* @brief Brightness control module.
*
* @author Diego Aceituno Seoane
*
* Controls the measurement of the photo-transistor for the brightness.
* This module is implemented taking into account that the voltage in the collector is 3.3 Volts.
* 
*/

/**
* @brief Reading of the analoge brightness sensor
* @return Float value as a percentage of the maximum possible value.
*/
extern float read_brightness_sensor_data();

#endif