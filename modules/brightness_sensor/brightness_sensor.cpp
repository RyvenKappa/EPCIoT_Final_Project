#include "mbed.h"
#include "brightness_sensor.h"
#include "../state_machine/control.h"

#define BRIGHTNESS_READ_PIN PA_4

Thread brightness_thread;

static AnalogIn value_reader(BRIGHTNESS_READ_PIN);

static ctrl_msg ctrl_msg_t;

/**
* Static function for the thread loop
*/
static void read_brightness_sensor_data(){
    ctrl_msg_t.type=BRIGHTNESS;
    ctrl_msg_t.brightness_msg = 0;
    while (true){
        ThisThread::flags_wait_all(BRIGHTNESS_SIGNAL);
        ctrl_msg_t.brightness_msg = ((float)value_reader.read_u16()*100)/0xFFFF;
        ctrl_in_queue.try_put_for(Kernel::wait_for_u32_forever, &ctrl_msg_t);
    }
}


void brightness_sensor_init(){
    brightness_thread.start(&read_brightness_sensor_data);
}