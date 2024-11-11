#ifndef I2C_INTERFACE_H
#define I2C_INTERFACE_H
#include "mbed.h"
/**
* @file i2c_interface.h
*
* @brief I2C interface reading control module.
*
* @author Diego Aceituno Seoane
*
* Defines the I2C thread to be used to call for the i2c sensor readings.
*
*/

#define I2C_SCL_PIN PB_8
#define I2C_SDA_PIN PB_9

extern Thread i2c_thread;

extern I2C i2c_bus;

/**
* @brief Inicialization of the i2c thread.
*/
extern void i2c_thread_init();


#endif