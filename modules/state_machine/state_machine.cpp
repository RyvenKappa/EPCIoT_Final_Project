#include "mbed.h"
#include "state_machine.h"
#include "../button/switch.h"
#include "../moisture_sensor/moisture_sensor.h"
#include "../brightness_sensor/brightness_sensor.h"
#include "../color_sensor/color_sensor.h"
#include "../temp_hum_sensor/temp_hum.h"
#include "../accelerometer_sensor/accelerometer.h"
#include "../gps_sensor/gps.h"
#include "../led/led_module.h"
#include "control.h"
#include "i2c_interface.h"
#include <cstdint>
#include <cstdio>

Queue<ctrl_msg, QUEUE_SIZE> ctrl_in_queue;

static BusOut board_leds(LED1,LED2,LED3);
I2C i2c_bus(I2C_SDA_PIN,I2C_SCL_PIN);

static ctrl_msg* ctrl_msg_t;

static float moisture = 0.0;
static float light = 0.0;
static int8_t sats = 0;
static float lat = 0.0;
static char lat_n = 0;
static float lng = 0.0;
static char lng_w = 0;
static int16_t altitude = 0;
static char altitude_c = 'M';
static int8_t time_h = 0;
static int8_t time_m = 0;
static int8_t time_s = 0;
static int8_t time_day = 0;
static int8_t time_month = 0;
static int16_t time_year = 0;
static uint16_t color_clear = 0;
static uint16_t color_red = 0;
static uint16_t color_green = 0;
static uint16_t color_blue = 0;
static float x_acc = 0.0;
static float y_acc = 0.0;
static float z_acc = 0.0;
static float temp = 0.0;
static float humidity = 0.0;

static LowPowerTicker ticker;
static LowPowerTimeout timeout;
static volatile bool timeout_event = false;
static volatile bool ticker_event = false;
/**
* Ticker ISR
*/
void ticker_isr(){
    ticker_event = true;
}
void timeout_isr(){
    timeout_event = true;
}


//****STATE MACHINE*******************
enum STATES{
    TEST,
    NORMAL,
    ADVANCED
};
static STATES actual_state;

void state_machine_init(){
    actual_state = TEST;
    board_leds.write(1);
    ticker.attach(ticker_isr,2000ms);
    timeout.attach(timeout_isr, 1700ms);
    i2c_bus.frequency(400000);
}
/**
* Auxiliar function to read data
*/
static void read_sensors_data(){
    while (ctrl_in_queue.try_get(&ctrl_msg_t)) {
        switch (ctrl_msg_t->type) {
            case ACCELEROMETER:
                x_acc = ctrl_msg_t->accelerometer_msg.x_acc;
                y_acc = ctrl_msg_t->accelerometer_msg.y_acc;
                z_acc = ctrl_msg_t->accelerometer_msg.z_acc;
                break;
            case GPS:
                time_h = ctrl_msg_t->gps_msg.time_h;
                time_m = ctrl_msg_t->gps_msg.time_m;
                time_s = ctrl_msg_t->gps_msg.time_s;
                time_day = ctrl_msg_t->gps_msg.time_day;
                time_month = ctrl_msg_t->gps_msg.time_month;
                time_year = ctrl_msg_t->gps_msg.time_year;
                lat = ctrl_msg_t->gps_msg.lat;
                lat_n = ctrl_msg_t->gps_msg.lat_n;
                lng = ctrl_msg_t->gps_msg.lng;
                lng_w = ctrl_msg_t->gps_msg.lng_w;
                altitude = ctrl_msg_t->gps_msg.altitude;
                altitude_c = ctrl_msg_t->gps_msg.altitude_c;
                sats = ctrl_msg_t->gps_msg.sats;
                break;
            case TEMP_HUM:
                temp = ctrl_msg_t->temp_hum_msg.temp;
                humidity = ctrl_msg_t->temp_hum_msg.hum;
                break;
            case COLOR:
                color_clear = ctrl_msg_t->color_msg.clear;
                color_red = ctrl_msg_t->color_msg.red;
                color_green = ctrl_msg_t->color_msg.green;
                color_blue = ctrl_msg_t->color_msg.blue;
                break;
        }
    }
    //Non thread messages
    moisture = read_moisture_sensor_data();
    light = read_brightness_sensor_data();
    printf("MODE: %d\n",actual_state);
    printf("SOIL MOISTURE: %.1f%%\n",moisture);
    printf("LIGHT: %.1f%%\n",light);
    printf("GPS: #Sats: %d Lat(UTC): %f %c Long(UTC): %f %c Altitude: %d %c GPS time: %d:%d:%d   Day%d, Month%d, Year%d\n",sats,lat,lat_n,lng,lng_w,altitude,altitude_c,time_h,time_m,time_s,time_day,time_month,time_year);
    printf("COLOR SENSOR: Clear: %d Red: %d Green: %d Blue: %d -- Dominant color: ",color_clear,color_red,color_green,color_blue);
    if ((color_red>color_blue) && (color_red>color_green)){
        printf("red\n");
        if (actual_state==TEST){
            change_led_color(true, false, false);
        }
    }else if ((color_blue>color_red) && (color_blue>color_green)) {
        printf("blue\n");
        if (actual_state==TEST){
            change_led_color(false, false, true);
        }
    }else if ((color_green>color_red) && (color_green>color_blue)) {
        printf("green\n");
        if (actual_state==TEST){
            change_led_color(false, true, false);
        }
    }else {
        printf("none\n");
        if (actual_state==TEST){
            change_led_color(false, false, false);
        }
    }
    printf("ACCELEROMETERS: X_axis: %.1f m/s², Y_axis: %.1f m/s², Z_axis: %.1f m/s²\n",x_acc,y_acc,z_acc);
    printf("TEMP/HUM: Temperature:\t%.1f °C,\tRelative Humidity: %.1f%%\n\n\n",temp,humidity);
}

void state_machine_cycle(){
    switch (actual_state) {
        case TEST:
            if(timeout_event){
                timeout_event = false;
                accelerometer_thread.flags_set(ACCELEROMETER_SIGNAL);
                temp_hum_thread.flags_set(TEMP_HUM_SIGNAL);
                color_thread.flags_set(COLOR_SIGNAL);
                gps_thread.flags_set(GPS_SIGNAL);
            }
            if(ticker_event){
                ticker_event=false;
                read_sensors_data();
                ticker.attach(ticker_isr,2000ms);
                timeout.attach(timeout_isr, 1700ms);

            }
            if(button_pressed_msg){
                button_pressed_msg = 0;
                actual_state = NORMAL;
                ticker.detach();
                timeout.detach();
                ticker.attach(ticker_isr,30000ms);
                timeout.attach(timeout_isr, 29700ms);
                board_leds.write(2);
                while(!ctrl_in_queue.empty()){
                    ctrl_in_queue.try_get(&ctrl_msg_t); //Empty possible previous messages
                }
                change_led_color(false, false, false);
                printf("Test a Normal\n");
            }
        break;
        case NORMAL:
            if(timeout_event){
                timeout_event = false;
                accelerometer_thread.flags_set(ACCELEROMETER_SIGNAL);
                temp_hum_thread.flags_set(TEMP_HUM_SIGNAL);
                color_thread.flags_set(COLOR_SIGNAL);
                gps_thread.flags_set(GPS_SIGNAL);
            }
            if(ticker_event){
                ticker_event=false;
                read_sensors_data();
                ticker.attach(ticker_isr,30000ms);
                timeout.attach(timeout_isr, 29700ms);

            }
            if (button_pressed_msg) {
                button_pressed_msg = 0;
                actual_state = ADVANCED;
                ticker.detach();
                timeout.detach();
                board_leds.write(4);
                while(!ctrl_in_queue.empty()){
                    ctrl_in_queue.try_get(&ctrl_msg_t); //Empty possible previous messages
                }
                printf("Normal a Advanced\n");
            }
        break;
        case ADVANCED:
            if (button_pressed_msg) {
                button_pressed_msg = 0;
                actual_state = TEST;
                ticker.attach(ticker_isr,2000ms);
                timeout.attach(timeout_isr, 1700ms);
                board_leds.write(1);
                while(!ctrl_in_queue.empty()){
                    ctrl_in_queue.try_get(&ctrl_msg_t); //Empty possible previous messages
                }
                printf("Advanced a Test\n");
            }
        break;
    }
}