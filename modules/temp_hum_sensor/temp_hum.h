#ifndef TEMP_HUM_H
#define TEMP_HUM_H
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

#define TH_SENSOR_SLAVE_ADDRESS 0x40

#define TH_SENSOR_MEASUREH_MASTER 0xE5
#define TH_SENSOR_MEASUREH_NMASTER 0xF5
#define TH_SENSOR_MEASURET_MASTER 0xE3
#define TH_SENSOR_MEASURET_NMASTER 0xF3
#define TH_SENSOR_MEASURET_PREVIOUS 0xE0
#define TH_SENSOR_RESET 0xFE
#define TH_SENSOR_WRITE_USER_REGISTER 0xE6
#define TH_SENSOR_READ_USER_REGISTER 0xE7
#define TH_SENSOR_WRITE_HEATER_REGISTER 0x51
#define TH_SENSOR_READ_HEATER_REGISTER 0x11

/**
* @brief Temp & Hum sensor reading
*/
extern void temp_hum_sensor_read();

#endif