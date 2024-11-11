#include "moisture_sensor.h"
#include "../state_machine/control.h"

#define MOISTURE_READ_PIN PA_0
#define MOISTURE_POWER_PIN PA_13

static AnalogIn value_reader(MOISTURE_READ_PIN);
static DigitalOut power_pin(MOISTURE_POWER_PIN);

/**
* Static function for the thread loop
*/
float read_moisture_sensor_data(){
    power_pin.write(1);
    ThisThread::sleep_for(1ms);
    float return_value = ((float)value_reader.read_u16()*100)/0xFFFF;
    power_pin.write(0);
    return return_value;
}