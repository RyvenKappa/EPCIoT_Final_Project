#include "moisture_sensor.h"
#include "../state_machine/control.h"

#define MOISTURE_READ_PIN PA_5
//Por ahora no le voy a meter la funcionalidad del drive por GPIO #define MOISTURE_POWER_PIN 

Thread moisture_thread(osPriorityNormal,256,nullptr,"MoistureThread");

static AnalogIn value_reader(MOISTURE_READ_PIN);

static ctrl_msg ctrl_msg_t;

/**
* Static function for the thread loop
*/
static void read_moisture_sensor_data(){
    ctrl_msg_t.type=MOISTURE;
    ctrl_msg_t.moisture_msg = 0;
    while(true){
        ThisThread::flags_wait_all(MOISTURE_SIGNAL);
        ctrl_msg_t.moisture_msg = ((float)value_reader.read_u16()*100)/0xFFFF;
        ctrl_in_queue.try_put_for(Kernel::wait_for_u32_forever, &ctrl_msg_t);
    }
}

void moisture_sensor_init(){
    moisture_thread.start(&read_moisture_sensor_data);
}

