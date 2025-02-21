/* 
 * File:   pwm.c
 * Author: Kyle James
 *
 * Created on February 21, 2025, 12:37 PM
 */
#include "pwm.h"
#include <avr/io.h>
#include "common.h"
#include "gpio.h"



void TCB2_init_pwm(int perc_duty_cycle) {
    
    // Set output pins MUX
    PORTMUX.TCBROUTEA &= ~(1 << 2); // TCB0 PWM on PC0
    
    TCB2.CTRLA = (0 << 0); // Disable timer for now, use DIV1 for CLK 

    TCB2.CTRLB = (1 << 4) | // Make output available on MUXed pin
                (0x7); // Use 8-bit PWM mode

}

void TCB2_set_duty_cycle(int perc_duty_cycle) {
    // Period
    uint8_t wave_per = F_CPU / (2 * PWM_FREQ) - 1;;
    uint8_t pulse_width = (uint8_t)((uint16_t)perc_duty_cycle * wave_per / 100);
    
    TCB2.CCMPL = wave_per;
    TCB2.CCMPH = pulse_width;
}

void TCB2_enable() {
    TCB2.CTRLA |= (1 << 0);
}

void TCB2_disable() {
    TCB2.CTRLA &= ~(1 << 0);
}