#include "mbed.h"
#include "modules/button/switch.h"
#include "modules/state_machine/state_machine.h"
#include "modules/moisture_sensor/moisture_sensor.h"

// main() runs in its own thread in the OS
int main()
{
    //Init leds
    button_init();
    state_machine_init();
    while (true) {
        state_machine_cycle();
    }
}

