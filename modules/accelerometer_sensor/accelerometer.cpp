#include "mbed.h"
#include "accelerometer.h"
#include "../state_machine/control.h"
#include "../state_machine/i2c_interface.h"

Thread accelerometer_thread(osPriorityNormal,256,nullptr,"AccThread");

static ctrl_msg ctrl_msg_t;


/**
* Static function for the thread loop
*/
static void read_accelerometer_data(){
    ctrl_msg_t.type=ACCELEROMETER;
    ctrl_msg_t.accelerometer_msg.x_acc = 0;
    ctrl_msg_t.accelerometer_msg.y_acc = 0;
    ctrl_msg_t.accelerometer_msg.z_acc = 0;
    while (true) {
        ThisThread::flags_wait_all(ACCELEROMETER_SIGNAL);
        //TODO read from the sensor
        ctrl_in_queue.try_put_for(Kernel::wait_for_u32_forever, &ctrl_msg_t);
    }
}

void accelerometer_sensor_init(){
    //TODO inicialize the sensor
    accelerometer_thread.start(&read_accelerometer_data);
}