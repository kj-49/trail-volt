/*
 * File:   main.c
 * Author: Kyle James
 *
 * Created on January 23, 2025, 9:50 AM
 */


#include <xc.h>
#define F_CPU 24000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include "avr-common.h"
#include "configuration.h"
#include "../lib/u8g2.X/u8g2.h"

#include <util/delay.h>

uint8_t u8x8_avr_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            // Configure SDA (PA1) and SCL (PA2) as output (change to your I2C pins)
            PORTA.DIRSET = (1 << 1) | (1 << 2);  // PA1 = SDA, PA2 = SCL
            break;
        
        case U8X8_MSG_DELAY_MILLI:
            while (arg_int--) {
                _delay_ms(1);
            }
            break;
        
        case U8X8_MSG_GPIO_I2C_CLOCK:
            if (arg_int) {
                PORTA.OUTSET = (1 << 2);  // Set SCL high
            } else {
                PORTA.OUTCLR = (1 << 2);  // Set SCL low
            }
            break;
        
        case U8X8_MSG_GPIO_I2C_DATA:
            if (arg_int) {
                PORTA.OUTSET = (1 << 1);  // Set SDA high
            } else {
                PORTA.OUTCLR = (1 << 1);  // Set SDA low
            }
            break;
        
        default:
            return 0;  // Not handled
    }
    return 1;  // Success
}


int main(void) {
    
    configure();
        
    u8g2_t u8g2; // a structure which will contain all the data for one display
    
    u8g2_Setup_sh1106_128x64_noname_1(&u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_avr_gpio_and_delay);  // init u8g2 structure
    u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
    u8g2_SetPowerSave(&u8g2, 0); // wake up display
    while(1) {
        
    }
    
    return 0;
}
