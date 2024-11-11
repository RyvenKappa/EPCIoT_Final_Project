#include "mbed.h"
#include "accelerometer.h"
#include "../state_machine/control.h"
#include "../i2c/i2c_interface.h"
#include <cstdint>
#include "stdio.h"

static ctrl_msg ctrl_msg_t;

static char cmd[2];
static char data[6];

/**
* Static function to read axis
* Acceleration is 14bits ->0b00DDDDDD DDDDDDDD
*/
static void read_acc_x_y_z(){
    cmd[0] = ACCELEROMETER_X_MSB;
    i2c_bus.write(ACCELEROMETER_SLAVE_ADDRESS<<1,cmd,1,true);
    i2c_bus.read(ACCELEROMETER_SLAVE_ADDRESS<<1,data,6);
    int16_t x_acc = (int16_t(data[0]<<8 | data[1]))>>2;
    int16_t y_acc = (int16_t(data[2]<<8 | data[3]))>>2;
    int16_t z_acc = (int16_t(data[4]<<8 | data[5]))>>2;
    ctrl_msg_t.accelerometer_msg.x_acc = ((float)x_acc)/1024.0;//TODO definir esto bien
    ctrl_msg_t.accelerometer_msg.y_acc = ((float)y_acc)/1024.0;
    ctrl_msg_t.accelerometer_msg.z_acc = ((float)z_acc)/1024.0;
}

/**
* Static function for the thread loop
*/
void accelerometer_sensor_read(){
    ctrl_msg_t.type=ACCELEROMETER;
    ctrl_msg_t.accelerometer_msg.x_acc = 0;
    ctrl_msg_t.accelerometer_msg.y_acc = 0;
    ctrl_msg_t.accelerometer_msg.z_acc = 0;
    //Set Active mode into the sensor
    cmd[0] = ACCELEROMETER_CTRL_REG1;
    cmd[1] = 0x01;//Active
    i2c_bus.write(ACCELEROMETER_SLAVE_ADDRESS<<1,cmd,2);
    ThisThread::sleep_for(5ms); //Turn-on time from standby

    read_acc_x_y_z();
    //Set Standby mode into the sensor
    cmd[0] = ACCELEROMETER_CTRL_REG1;
    cmd[1] = 0x00;//Standby
    i2c_bus.write(ACCELEROMETER_SLAVE_ADDRESS<<1,cmd,2);
    ctrl_in_queue.try_put_for(Kernel::wait_for_u32_forever, &ctrl_msg_t);
}

void accelerometer_sensor_init(){
    //Reset of the sensor
    cmd[0] = ACCELEROMETER_CTRL_REG2;
    cmd[1] = 0x40;
    i2c_bus.write(ACCELEROMETER_SLAVE_ADDRESS<<1,cmd,2);
    //We wait for the software reset
    ThisThread::sleep_for(3ms);

    //To configure some things as Low noise, we need to be in standby
    //Standby
    cmd[0] = ACCELEROMETER_F_SETUP;
    cmd[1] = 0x00;
    i2c_bus.write(ACCELEROMETER_SLAVE_ADDRESS<<1,cmd,2);
    //Configure 8G dynamic range
    cmd[0] = ACCELEROMETER_XYZ_DATA_CFG;
    cmd[1] = 0x02;
    i2c_bus.write(ACCELEROMETER_SLAVE_ADDRESS<<1,cmd,2);

    cmd[0] = ACCELEROMETER_CTRL_REG1;
    cmd[1] = 0x01;
    i2c_bus.write(ACCELEROMETER_SLAVE_ADDRESS<<1,cmd,2);
    //Low noise is only 4G, command:
    //cmd[1] = 0x05;
    //i2c_bus.write(ACCELEROMETER_SLAVE_ADDRESS<<1,cmd,2);
}