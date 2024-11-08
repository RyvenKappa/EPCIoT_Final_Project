#include "mbed.h"
#include "led_module.h"
#include <memory>

#ifndef DEBUG
#define LED_B PA_14
#endif

#define LED_G PH_0 // 
#define LED_R PH_1//


static DigitalOut red(LED_R);
static DigitalOut green(LED_G);
#ifndef DEBUG
static DigitalOut blue(LED_B);
#endif

void led_init(){

    #ifndef DEBUG
    blue.write(1);
    #endif

    green.write(1);
    red.write(1);
}

void change_led_color(bool r, bool g, bool b){

    #ifndef DEBUG
    blue = (b==1)? 0 : 1;
    #endif

    green = (g==1)? 0 : 1;
    red = (r==1)? 0 : 1;
}