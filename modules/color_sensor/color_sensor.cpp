#include "mbed.h"
#include "color_sensor.h"
#include "../state_machine/control.h"
#include "../i2c/i2c_interface.h"
#include <cstdint>
#include <stdint.h>

DigitalOut color_led(COLOR_SENSOR_LED);
InterruptIn color_int(COLOR_SENSOR_INT);

volatile int color_sensor_message = false;

/**
* ISR for the color sensor interruption
* It will always be saved in order to be cleaned when entering advanced mode
*/
void color_sensor_handler(){
    color_sensor_message = true;
}

static ctrl_msg ctrl_msg_t;

static char cmd[2];
static char data[8];
/**
* Static function for measurement processing
*/
static void measurement_process(){
    ctrl_msg_t.color_msg.clear = data[1]<<8 | data[0];
    ctrl_msg_t.color_msg.red = data[3]<<8 | data[2];
    ctrl_msg_t.color_msg.green = data[5]<<8 | data[4];
    ctrl_msg_t.color_msg.blue = data[7]<<8 | data[6];
    //Ask if compensation is needed to take into account the sensor response to different light longitudes
}


/**
* Static function for the thread loop
*/
void color_sensor_read(){
    ctrl_msg_t.type=COLOR;
    ctrl_msg_t.color_msg.blue=0;
    ctrl_msg_t.color_msg.clear=0;
    ctrl_msg_t.color_msg.green=0;
    ctrl_msg_t.color_msg.red=0;
    color_led.write(1);
    //Activate sensor, AEN & PON
    cmd[0] = COLOR_SENSOR_CMD_MSK | COLOR_SENSOR_TYPE_AUTO_INCREMENT | COLOR_SENSOR_ENABLE;
    cmd[1] = 0x03;
    i2c_bus.write(COLOR_SENSOR_SLAVE_ADDRESS<<1,cmd,2);
    ThisThread::sleep_for(3ms); //PON stabilization time
    //Wait for integration and reading
    ThisThread::sleep_for(160ms); //Integration time and RGBC Init 2.4+154=157.4
    color_led.write(0);
    cmd[0] = COLOR_SENSOR_CMD_MSK | COLOR_SENSOR_TYPE_AUTO_INCREMENT | COLOR_SENSOR_CDATAL;
    i2c_bus.write(COLOR_SENSOR_SLAVE_ADDRESS<<1,cmd,1);
    i2c_bus.read(COLOR_SENSOR_SLAVE_ADDRESS<<1,data,8);
    //Deactivate sensor and disable RGBC ADCs
    cmd[0] = COLOR_SENSOR_CMD_MSK | COLOR_SENSOR_TYPE_AUTO_INCREMENT | COLOR_SENSOR_ENABLE;
    cmd[1] = 0x00;//!AEN & !PON
    i2c_bus.write(COLOR_SENSOR_SLAVE_ADDRESS<<1,cmd,2);
    //Process measurement into the ctrl_msg_t and put into queue
    measurement_process();
    ctrl_in_queue.try_put_for(Kernel::wait_for_u32_forever, &ctrl_msg_t);
}


void color_sensor_init(){
    color_int.rise(&color_sensor_handler);
    cmd[0] = COLOR_SENSOR_CMD_MSK | COLOR_SENSOR_TYPE_AUTO_INCREMENT | COLOR_SENSOR_ENABLE;
    cmd[1] = 0x02; //Wait dissabled, interruptions dissabled and AEN deactivated
    i2c_bus.write(COLOR_SENSOR_SLAVE_ADDRESS<<1,cmd,2);
    cmd[0] = COLOR_SENSOR_CMD_MSK | COLOR_SENSOR_TYPE_AUTO_INCREMENT | COLOR_SENSOR_ATIME;
    cmd[1] = 0xC0;//154ms of RGBC Integration
    i2c_bus.write(COLOR_SENSOR_SLAVE_ADDRESS<<1,cmd,2);
}