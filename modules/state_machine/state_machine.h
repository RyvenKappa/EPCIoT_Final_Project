#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
#include <cstdint>
#include <stdint.h>

/**
* @file state_machine.h
*
* @brief State_Machine main module.
*
* @author Diego Aceituno Seoane
*
* Module that implements an state_machine to control the events of the system.
* Controls the funcionality changes between states.
*
*/

/**
* Function to setup:
*   - Initial State of the system.
*   - Board Leds initial state.
*/
extern void state_machine_init();

/**
* Function to do one cycle of the state_machine.
*/
extern void state_machine_cycle();
#endif