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
*
*/

/**
* @brief Measurement thread for the moisture sensor
*/
extern Thread moisture_thread; 

/**
* @brief Inicialization of the moisture_sensor_thread
*/
extern void  moisture_sensor_init();

#endif