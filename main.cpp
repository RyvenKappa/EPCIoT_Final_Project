#include "mbed.h"
#include "modules/button/switch.h"
#include "modules/state_machine/state_machine.h"
#include "modules/moisture_sensor/moisture_sensor.h"
#include "modules/color_sensor/color_sensor.h"
#include "modules/temp_hum_sensor/temp_hum.h"
#include "modules/accelerometer_sensor/accelerometer.h"

// main() runs in its own thread in the OS
int main()
{
    button_init();
    state_machine_init();
    moisture_sensor_init();
    color_sensor_init();
    temp_hum_sensor_init();
    accelerometer_sensor_init();
    while (true) {
        state_machine_cycle();
    }
}

