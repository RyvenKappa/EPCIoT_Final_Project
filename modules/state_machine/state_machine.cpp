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
#include "../i2c/i2c_interface.h"
#include "control.h"

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

static uint8_t hour_limit_counter = 0; //This counter goes up by 1 every 30 seconds on NORMAL and ADVANCED mode.

static bool started = false;

//Mean, Maximum and Minimum values
static float min_temp = 0.0;
static float max_temp = 0.0;
static float temp_adder = 0.0;

static float min_humidity = 0.0;
static float max_humidity = 0.0;
static float humidity_adder = 0.0;

static float min_light = 0.0;
static float max_light = 0.0;
static float light_adder = 0.0;

static float min_moisture = 0.0;
static float max_moisture = 0.0;
static float moisture_adder = 0.0;

//Minimum and maximum values only for acceleration
static float min_x = 0.0;
static float max_x = 0.0;
static float min_y = 0.0;
static float max_y = 0.0;
static float min_z = 0.0;
static float max_z = 0.0;

//Dominant color on the plant
static uint8_t red_ocurrences = 0;
static uint8_t green_ocurrences = 0;
static uint8_t blue_ocurrences = 0;


//****STATE MACHINE*******************
enum STATES{
    TEST,
    NORMAL,
    ADVANCED
};

static char states_names[3][10] = {"TEST","NORMAL","ADVANCED"}; 

static STATES actual_state;

static LowPowerTicker ticker;
static LowPowerTimeout timeout;
static volatile bool timeout_event = false;
static volatile bool ticker_event = false;

static bool sleep_ready = true;

/**
* Ticker ISR
*/
void ticker_isr(){
    ticker_event = true;
}
void timeout_isr(){
    timeout_event = true;
    if (actual_state==ADVANCED) {
        SCB->SCR &= ~(SCB_SCR_SLEEPONEXIT_Msk);
    }
}

void state_machine_init(){
    actual_state = TEST;
    board_leds.write(1);
    ticker.attach(ticker_isr,2000ms);
    timeout.attach(timeout_isr, 1600ms);
    i2c_bus.frequency(400000);
}

static void reset_temp_data(){
    hour_limit_counter=0;
    min_temp = 0;
    max_temp = 0;
    temp_adder = 0;
    min_humidity = 0;
    max_humidity = 0;
    humidity_adder = 0;
    min_light = 0;
    max_light = 0;
    light_adder = 0;
    min_moisture = 0;
    max_moisture = 0;
    moisture_adder = 0;
    min_x = 0;
    max_x = 0;
    min_y = 0;
    max_y = 0;
    min_z = 0;
    max_z = 0;
    red_ocurrences = 0;
    green_ocurrences = 0;
    blue_ocurrences = 0;
    started = false;
}
/**
* Auxiliar function to print the mean, min and max values of the variables to the serial.
*/
static void hour_data_to_serial(){
    printf("1 Hour has passed, this is the data analysis:\n");
    printf("\tTemperature:   min:%.1f°C, max:%.1f°C, mean:%.1f°C.\n",min_temp,max_temp,(temp_adder/hour_limit_counter));
    printf("\tHumidity:   min:%.1f%%, max:%.1f%%, mean:%.1f%%.\n",min_humidity,max_humidity,(humidity_adder/hour_limit_counter));
    printf("\tLight:   min:%.1f%%, max:%.1f%%, mean:%.1f%%.\n",min_light,max_light,(light_adder/hour_limit_counter));
    printf("\tMoisture:   min:%.1f%% max:%.1f%%, mean:%.1f%%.\n",min_moisture,max_moisture,(moisture_adder/hour_limit_counter));
    printf("\tAccelerometer_X:   min:%.1fm/s², max:%.1fm/s².\n",min_x,max_x);
    printf("\tAccelerometer_Y:   min:%.1fm/s², max:%.1fm/s².\n",min_y,max_y);
    printf("\tAccelerometer_Z:   min:%.1fm/s², max:%.1fm/s².\n",min_z,max_z);
    //Most dominant color
    if ((red_ocurrences>green_ocurrences) && (red_ocurrences>blue_ocurrences)){
        printf("\tMost dominant color: red\n");
    }else if ((green_ocurrences>red_ocurrences) && (green_ocurrences>blue_ocurrences)) {
        printf("\tMost dominant color: green\n");
    }else if ((blue_ocurrences>red_ocurrences) && (blue_ocurrences>green_ocurrences)) {
        printf("\tMost dominant color: blue\n");
    }else{
        printf("\tMost dominant color: none\n");
    }
    reset_temp_data();
}

/**
* Auxiliar function to print the standard message to the serial when no emergency.
*/
static void data_to_serial(){
    printf("MODE: %s\n\n",states_names[actual_state]);;
    if (actual_state!=TEST){   
        change_led_color(false, false, false);
        //Temp limits
        //Red if problem
        if ((temp<10.0) || (temp>35.0)){
            printf("TEMPERATURE %.1f°C EXCEDING LIMITS, lower or raise the air temperature of the room!\n",temp);
            change_led_color(true, false, false);
        }
        //Humidity limits
        //Blue if problem
        if((humidity<25) || (humidity>75.0)){
            printf("HUMIDITY %.1f%% EXCEDING LIMITS, increase or reduce the air flow of the room!\n",humidity);
            change_led_color(false, false, true);
        }
        //Light limits
        //Yellow if problem with light.
        if(light<10){
            printf("LIGHT %.1f%% TOO LOW!\n",light);
            change_led_color(true, true, false);
        }
        if (light>80){
            printf("LIGHT %.1f%% TOO BRIGHT!\n",light);
            change_led_color(true, true, false);
        }
        //Moisture limits
        //Purple if problem with moisture.
        if(moisture<5){
            printf("MOISTURE %.1f%% TOO LOW!\n",moisture);
            change_led_color(true, false, true);
        }
        if (moisture>85){
            printf("MOISTURE %.1f%% TOO HIGH!\n",moisture);
            change_led_color(true, false, true);
        }
        //Color problems
        //Whie if problem with color sensor, detecting more blue.
        if((color_blue>color_red) && (color_blue>color_green)){
            printf("COLOR IS BLUE, CHECK COLOR SENSOR!\n");
            change_led_color(true, true, true);
        }
        //ACELERATION TODO CRASH
        printf("\n");
    }
    //Print typical message
    printf("\tSOIL MOISTURE: %.1f%%\n",moisture);
    printf("\tLIGHT: %.1f%%\n",light);
    printf("\tGPS: #Sats: %d Lat(UTC): %f %c Long(UTC): %f %c Altitude: %d %c GPS time: %d:%d:%d CET\tDay %d, Month %d, Year %d\n",sats,lat,lat_n,lng,lng_w,altitude,altitude_c,time_h,time_m,time_s,time_day,time_month,time_year);
    printf("\tCOLOR SENSOR: Clear: %d Red: %d Green: %d Blue: %d -- Dominant color: ",color_clear,color_red,color_green,color_blue);
    //Calculate dominant color
    if ((color_red>color_blue) && (color_red>color_green)){
        printf("red\n");
        if (actual_state==TEST){
            change_led_color(true, false, false);
        }else{
            red_ocurrences += 1;
        }
    }else if ((color_blue>color_red) && (color_blue>color_green)) {
        printf("blue\n");
        if (actual_state==TEST){
            change_led_color(false, false, true);
        }else{
            blue_ocurrences += 1;
        }
    }else if ((color_green>color_red) && (color_green>color_blue)) {
        printf("green\n");
        if (actual_state==TEST){
            change_led_color(false, true, false);
        }else{
            green_ocurrences += 1;
        }
    }else {
        printf("none\n");
        if (actual_state==TEST){
            change_led_color(false, false, false);
        }
    }

    printf("\tACCELEROMETERS: X_axis: %.1f m/s², Y_axis: %.1f m/s², Z_axis: %.1f m/s²\n",x_acc,y_acc,z_acc);
    printf("\tTEMP/HUM: Temperature:\t%.1f °C,\tRelative Humidity: %.1f%%\n\n\n",temp,humidity);
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
    moisture = read_moisture_sensor_data();
    light = read_brightness_sensor_data();
    if (!started){
        //Assign min and max
        min_x = x_acc;
        max_x = x_acc;
        min_y = y_acc;
        max_y = y_acc;
        min_z = z_acc;
        max_z = z_acc;
        min_temp = temp;
        max_temp = temp;
        min_humidity = humidity;
        max_humidity = humidity;
        min_moisture = moisture;
        max_moisture = moisture;
        min_light = light;
        max_light = light;
        started = true;
    }

    //Max and min values checking
    if (actual_state != TEST){
        //Acceleration, min and max value over hour
        if(x_acc>max_x){max_x = x_acc;}else if (x_acc<min_x) {min_x = x_acc;}
        if(y_acc>max_y){max_y = y_acc;}else if (y_acc<min_y) {min_y = y_acc;}
        if(z_acc>max_z){max_z = z_acc;}else if (z_acc<min_z) {min_z = z_acc;}
        //Temperature
        if(temp>max_temp){max_temp = temp;}else if (temp<min_temp) {min_temp = temp;}
        temp_adder += temp;
        //Humidity
        if(humidity>max_humidity){max_humidity = humidity;}else if (humidity<min_humidity) {min_humidity = humidity;}
        humidity_adder += humidity;
        //Humidity
        if(light>max_light){max_light = light;}else if (light<min_light) {min_light = light;}
        light_adder += light;
        //Moisture
        if(moisture>max_moisture){max_moisture = moisture;}else if (moisture<min_moisture) {min_moisture = moisture;}
        moisture_adder += moisture;
    }
}


/**
* Inline(every call pastes the same code) function to control the sleep of the MCU
*/
inline void deep_sleep_stop(){
    if(sleep_ready){
        /*Prepare the entering to sleep mode*/
        PWR->CR |= PWR_CR_CWUF_Msk; //Clear the wake up flag after 2 clock cycles.
        PWR->CR &= ~( PWR_CR_PDDS); //Enter stop mode when the cpu enters deepsleep

        SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk); //Allow deepsleep
        SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;  //Reenter low power after ISR
    }
    __WFI();
    
}


void state_machine_cycle(){
    switch (actual_state) {
        case TEST:
            if(timeout_event){
                timeout_event = false;
                i2c_thread.flags_set(I2C_SIGNAL);
                gps_thread.flags_set(GPS_SIGNAL);
            }
            if(ticker_event){
                ticker_event=false;
                read_sensors_data();
                data_to_serial();
                ticker.attach(ticker_isr,2000ms);
                timeout.attach(timeout_isr, 1600ms);
            }
            if(button_pressed_msg){
                button_pressed_msg = 0;
                actual_state = NORMAL;
                ticker.detach();
                timeout.detach();
                ticker.attach(ticker_isr,30000ms);
                timeout.attach(timeout_isr, 29600ms);
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
                i2c_thread.flags_set(I2C_SIGNAL);
                gps_thread.flags_set(GPS_SIGNAL);
            }
            if(ticker_event){
                ticker_event=false;
                read_sensors_data();
                hour_limit_counter += 1;
                data_to_serial();
                if (hour_limit_counter==120) {
                    hour_data_to_serial();
                }
                ticker.attach(ticker_isr,30000ms);
                timeout.attach(timeout_isr, 29600ms);

            }
            if (button_pressed_msg) {
                button_pressed_msg = 0;
                actual_state = ADVANCED;
                ticker.detach();
                timeout.detach();
                ticker.attach(ticker_isr,30000ms);
                timeout.attach(timeout_isr, 29600ms);
                board_leds.write(4);
                while(!ctrl_in_queue.empty()){
                    ctrl_in_queue.try_get(&ctrl_msg_t); //Empty possible previous messages
                }
                reset_temp_data();
                printf("Normal a Advanced\n");
            }
        break;
        case ADVANCED:
            if(timeout_event){
                timeout_event = false;
                //Stop mode configuration is taken out
                PWR->CR &= ~(PWR_CR_CWUF_Msk);
                PWR->CR |= PWR_CR_PDDS;
                sleep_ready = false;
                i2c_thread.flags_set(I2C_SIGNAL);
                gps_thread.flags_set(GPS_SIGNAL);
            }
            if(ticker_event){
                ticker_event=false;
                read_sensors_data();
                hour_limit_counter += 1;
                data_to_serial();
                if (hour_limit_counter==120) {
                    hour_data_to_serial();
                }
                ticker.attach(ticker_isr,30000ms);
                timeout.attach(timeout_isr, 29600ms);
                sleep_ready = true;
            }
            if (button_pressed_msg) {
                button_pressed_msg = 0;
                actual_state = TEST;
                ticker.attach(ticker_isr,2000ms);
                timeout.attach(timeout_isr, 1600ms);
                board_leds.write(1);
                while(!ctrl_in_queue.empty()){
                    ctrl_in_queue.try_get(&ctrl_msg_t); //Empty possible previous messages
                }
                reset_temp_data();
                printf("Advanced a Test\n");
            }
            deep_sleep_stop();
        break;
    }
}