#include "mbed.h"
#include "brightness_sensor.h"
#include "../state_machine/control.h"

#define BRIGHTNESS_READ_PIN PA_4

static AnalogIn value_reader(BRIGHTNESS_READ_PIN);


/**
* Static function for the thread loop
*/
float read_brightness_sensor_data(){
    return ((float)value_reader.read_u16()*100)/0xFFFF;
}