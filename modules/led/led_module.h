#ifndef LED_MODULE_H
#define LED_MODULE_H

/**
* @file led_module.h
*
* @brief RGB Led control module.
*
* @author Diego Aceituno Seoane
*
* Controls an RGB Led with three digital color components.
*
*/

//#define DEBUG

/**
*
* RGB Led inicialization.
*
*/
extern void led_init();

/**
*
* Changes the RGB Led color.
* @param red boolean value that turns on or off the red component.
* @param green boolean value that turns on or off the green component.
* @param blue boolean value that turns on or off the blue component.
*
*/
extern void change_led_color(bool red, bool green, bool blue);


#endif
