/* 
 * File:   interrupts.c
 * Author: Kyle James
 *
 * Created on February 17, 2025, 10:12 PM
 */

#include "interrupts.h"
#include <avr/io.h>


void configure_interrupts() {
    
    sei();
    
    // Pull-up on wake up pin
    PORTB.PIN0CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
    
}
