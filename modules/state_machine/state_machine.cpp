#include "mbed.h"
#include "state_machine.h"
#include "../button/switch.h"

enum STATES{
    TEST,
    NORMAL,
    ADVANCED
};

static STATES actual_state;
static BusOut board_leds(LED1,LED2,LED3);

void state_machine_init(){
    actual_state = TEST;
    board_leds.write(1);
}

void state_machine_cycle(){
    switch (actual_state) {
        case TEST:
            if(button_pressed_msg){
                button_pressed_msg = 0;
                actual_state = NORMAL;
                board_leds.write(2);
                printf("Test a Normal\n");
            }
        break;
        case NORMAL:
            if (button_pressed_msg) {
                button_pressed_msg = 0;
                actual_state = ADVANCED;
                board_leds.write(4);
                printf("Normal a Advanced\n");
            }
        break;
        case ADVANCED:
            if (button_pressed_msg) {
                button_pressed_msg = 0;
                actual_state = TEST;
                board_leds.write(1);
                printf("Advanced a Test\n");
            }
        break;
    }
}