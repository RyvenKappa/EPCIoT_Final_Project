#ifndef INTERFACES_H
#define INTERFACES_H
#include "mbed.h"
/**
* @file interfaces.h
*
* @brief I2C and UART control module.
*
* @author Diego Aceituno Seoane
*
* Defines the I2C object to be used as communication interfaces for different sensors.
*
*/

#define I2C_SCL_PIN PB_8
#define I2C_SDA_PIN PB_9

extern I2C i2c_bus;


#endif