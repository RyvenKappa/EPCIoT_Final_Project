#include "mbed.h"
#include "state_machine.h"
#include "../button/switch.h"
#include "../moisture_sensor/moisture_sensor.h"
#include "../brightness_sensor/brightness_sensor.h"
#include "../color_sensor/color_sensor.h"
#include "../temp_hum_sensor/temp_hum.h"
#include "../accelerometer_sensor/accelerometer.h"
#include "control.h"
#include "i2c_interface.h"

Queue<ctrl_msg, QUEUE_SIZE> ctrl_in_queue;

enum STATES{
    TEST,
    NORMAL,
    ADVANCED
};

static STATES actual_state;

static BusOut board_leds(LED1,LED2,LED3);
I2C i2c_bus(I2C_SDA_PIN,I2C_SCL_PIN);

static ctrl_msg* ctrl_msg_t;


static Ticker ticker;
static volatile bool ticker_event = false;

/**
* Ticker ISR
*/
void ticker_handler(){
    ticker_event = true;
}

void state_machine_init(){
    actual_state = TEST;
    board_leds.write(1);
    ticker.attach(ticker_handler,5s);
}

void state_machine_cycle(){
    switch (actual_state) {
        case TEST:
            if(ticker_event){
                ticker_event=false;
                moisture_thread.flags_set(MOISTURE_SIGNAL);
                brightness_thread.flags_set(BRIGHTNESS_SIGNAL);
                //accelerometer_thread.flags_set(ACCELEROMETER_SIGNAL);
            }
            if(ctrl_in_queue.try_get(&ctrl_msg_t)){
                if (ctrl_msg_t->type==MOISTURE){
                    printf("Value of the moisture: %f\n",ctrl_msg_t->moisture_msg);
                }else if (ctrl_msg_t->type==BRIGHTNESS) {
                    printf("Value of the brightness: %f\n",ctrl_msg_t->brightness_msg);
                }
            }

            if(button_pressed_msg){
                button_pressed_msg = 0;
                actual_state = NORMAL;
                ticker.detach();
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