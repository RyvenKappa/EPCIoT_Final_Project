#include "i2c_interface.h"
#include "../state_machine/control.h"
#include "../temp_hum_sensor/temp_hum.h"
#include "../accelerometer_sensor/accelerometer.h"
#include "../color_sensor/color_sensor.h"

Thread i2c_thread(osPriorityNormal,256,nullptr,"I2CThread");

/**
* Static function to call for the readings of the i2c sensors.
*/
static void i2c_read_sensors(){
    while (true) {
        ThisThread::flags_wait_all(I2C_SIGNAL);
        accelerometer_sensor_read();
        color_sensor_read();
        temp_hum_sensor_read();
    }
}


void i2c_thread_init(){
    accelerometer_sensor_init();
    color_sensor_init();
    i2c_thread.start(&i2c_read_sensors);
}