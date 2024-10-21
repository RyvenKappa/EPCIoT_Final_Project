#include "mbed.h"
#include "led_module.h"
#include <memory>

#define LED_R PA_14//
#define LED_G PH_1 // 
#define LED_B PH_0//

static DigitalOut red(LED_R);
static DigitalOut green(LED_G);
static DigitalOut blue(LED_B);

void led_init(){
    red.write(1);
    green.write(1);
    blue.write(1);
}

void change_led_color(bool r, bool g, bool b){
    red = (r==1)? 0 : 1;
    green = (g==1)? 0 : 1;
    blue = (b==1)? 0 : 1;
}