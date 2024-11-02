#include "mbed.h"
#include "temp_hum.h"
#include "../state_machine/i2c_interface.h"
#include "../state_machine/control.h"

Thread temp_hum_thread(osPriorityNormal,256);

static ctrl_msg ctrl_msg_t;

/**
* Static function for the thread loop
*/
static void read_temp_hum_sensor_data(){
    ctrl_msg_t.type=TEMP_HUM;
    ctrl_msg_t.temp_hum_msg.temp = 0;
    ctrl_msg_t.temp_hum_msg.hum = 0;
    while (true) {
        ThisThread::flags_wait_all(TEMP_HUM_SIGNAL);
        //TODO Obtain measurements
        ctrl_in_queue.try_put_for(Kernel::wait_for_u32_forever, &ctrl_msg_t);
    }
}

void temp_hum_sensor_init(){
    temp_hum_thread.start(&read_temp_hum_sensor_data);
}