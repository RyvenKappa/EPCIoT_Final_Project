#include "mbed.h"
#include "modules/led/led_module.h"
#include "modules/button/switch.h"
#include "modules/state_machine/state_machine.h"
#include "modules/i2c/i2c_interface.h"
#include "modules/gps_sensor/gps.h"

// main() runs in its own thread in the OS
int main()
{
    led_init();
    button_init();
    state_machine_init();
    i2c_thread_init();
    gps_init();
    while (true) {
        state_machine_cycle();
    }
}

