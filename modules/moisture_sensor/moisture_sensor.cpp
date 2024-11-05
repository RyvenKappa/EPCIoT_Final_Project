#include "moisture_sensor.h"
#include "../state_machine/control.h"

#define MOISTURE_READ_PIN PA_5
//TODO Por ahora no le voy a meter la funcionalidad del drive por GPIO #define MOISTURE_POWER_PIN 

static AnalogIn value_reader(MOISTURE_READ_PIN);

/**
* Static function for the thread loop
*/
float read_moisture_sensor_data(){
    return ((float)value_reader.read_u16()*100)/0xFFFF;
}
