#ifndef SWITCH_H
#define SWITCH_H
/**
* @file switch.h
*
* @brief Modulo del boton.
*
* @author Diego Aceituno Seoane
*
* Controla el boton de usuario y registra las interrupciones necesarias.
* El boton ya tiene controlado los rebotes por diseño HardWare.
*
*/

extern volatile int button_pressed_msg;


/**
*
* Registro de la interrupción y mensajes relacionados con el boton
*
*/
void button_init();

#endif