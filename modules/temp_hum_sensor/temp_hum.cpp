#include "mbed.h"
#include "temp_hum.h"
#include "../i2c/i2c_interface.h"
#include "../state_machine/control.h"
#include <cstdint>
#include <stdint.h>

static ctrl_msg ctrl_msg_t;

static char cmd;
static char buff[2];

/**
* Reading function with conversion
*/
static void converted_reading(){
    cmd = TH_SENSOR_MEASUREH_MASTER;
    i2c_bus.write(TH_SENSOR_SLAVE_ADDRESS<<1,&cmd,1,true);
    //Humidity
    i2c_bus.read(TH_SENSOR_SLAVE_ADDRESS<<1,buff,2);
    //conversion
    ctrl_msg_t.temp_hum_msg.hum = ((125.0*(buff[0]<<8 | buff[1]))/65536.0)-6;


    //Obtain last temperature measurement
    cmd = TH_SENSOR_MEASURET_PREVIOUS;
    i2c_bus.write(TH_SENSOR_SLAVE_ADDRESS<<1,&cmd,1);
    i2c_bus.read(TH_SENSOR_SLAVE_ADDRESS<<1,buff,2);
    //Conversion
    ctrl_msg_t.temp_hum_msg.temp = (175.72*(buff[0]<<8 | buff[1])/65536.0)-46.85;
}

/**
* Static function for the thread loop
*/
void temp_hum_sensor_read(){
    ctrl_msg_t.type=TEMP_HUM;
    ctrl_msg_t.temp_hum_msg.temp = 0;
    ctrl_msg_t.temp_hum_msg.hum = 0;
    converted_reading();
    ctrl_in_queue.try_put_for(Kernel::wait_for_u32_forever, &ctrl_msg_t);
}
