#ifndef GPS_H
#define GPS_H
#include "mbed.h"
/**
* @file gps.h
*
* @brief GPS control module.
*
* @author Diego Aceituno Seoane
*
* Controls a connection with a GPS sensor and the UART communication.
*
*/

#define GPS_RX PA_10

extern Thread gps_thread;

/**
* @brief Inicialization of the GPS thread
*/
extern void gps_init();

#endif