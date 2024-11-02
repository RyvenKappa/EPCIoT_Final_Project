#include "mbed.h"
#include "temp_hum.h"
#include "mbed.h"
/**
* @file temp_hum.h
*
* @brief Temperature and Humidity sensor control module.
*
* @author Diego Aceituno Seoane
*
* Controls a Si7021-A20 temperature and Humidity sensor
*
*/


/**
* @brief Measurement thread for the temperature and humidity sensor.
*/
extern Thread temp_hum_thread;

/**
* @brief Inicialization of the temperature and humidity thread.
*/
extern void temp_hum_sensor_init();