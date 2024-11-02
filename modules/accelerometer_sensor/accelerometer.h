#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H
#include "mbed.h"
/**
* @file accelerometer.h
*
* @brief Accelerometer control module.
*
* @author Diego Aceituno Seoane
*
* Controls a MMA8451Q 3-Axis sensor
*
*/

/**
* @brief Measurement thread for the accelerometer sensor.
*/
extern Thread accelerometer_thread;

/**
* @brief Inicialization of the accelerometer thread
*/
extern void accelerometer_sensor_init();
#endif