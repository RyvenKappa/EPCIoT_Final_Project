#ifndef MOISTURE_SENSOR_H
#define MOISTURE_SENSOR_H
#include "mbed.h"
/**
* @file moisture_sensor.h
*
* @brief Moisture sensor control module.
*
* @author Diego Aceituno Seoane
*
* Controls a SparkFun soil moisture sensor.
* This module powers off and on the digital pin in order to reduce wearness over time.
*
*/

/**
* @brief Reading of the analoge moisture sensor
* @return Float value as a percentage of the maximum possible value.
*/
extern float read_moisture_sensor_data();

#endif