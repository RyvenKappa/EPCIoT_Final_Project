#include "mbed.h"
#include "switch.h"
#include <memory>

#define USER_SWITCH PB_2

static InterruptIn sw(USER_SWITCH);

volatile int button_pressed_msg = 0;
volatile int button_released_msg = 0;


void switch_pressed_handler(){
    button_pressed_msg = 1;
}

void switch_released_handler(){
    button_released_msg = 1;
}

void button_init(){
    sw.mode(PullNone);
    sw.fall(&switch_pressed_handler);
    sw.rise(&switch_released_handler);
}