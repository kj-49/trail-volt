/* 
 * File:   configuration.c
 * Author: Kyle James
 *
 * Created on January 23, 2025, 8:51 PM
 */
#include <avr/io.h>
#include "configuration.h"

void set_pin_directions();

void configure() {
    
    set_pin_directions();
    
}

void set_pin_directions() {
    
    // Configure input pins
    PORTA.DIRSET = (1 << A_LED_CHECK);
}