/* 
 * File:   display.c
 * Author: Kyle James
 *
 * Created on February 15, 2025, 6:39 PM
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "display.h"

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_avr_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

void configure_display(u8g2_t *u8g2) {

    // Initialize display (SH1106 128x64)
    u8g2_Setup_sh1106_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_avr_gpio_and_delay);
    
    // Set I2C address for the SH1106 display (0x3C or 0x3D depending on the display)
    u8g2_SetI2CAddress(u8g2, 0x3C << 1);  // Shift left to match 7-bit address format
    u8g2_InitDisplay(u8g2);
    u8g2_SetPowerSave(u8g2, 0);  // Wake up the display
    
    u8g2_SetFont(u8g2, u8g2_font_littlemissloudonbold_tr);
}
void update_display(u8g2_t *u8g2, sensor_state_t *sensor_state) {
    char buffer[20]; // Buffer to hold formatted strings

    // Clear the display buffer
    u8g2_ClearBuffer(u8g2);

    // Display Cell 1 Voltage
    snprintf(buffer, sizeof(buffer), "Cell 1: %d mV", sensor_state->cell1_voltage);
    u8g2_DrawStr(u8g2, 0, 10, buffer);

    // Display Cell 2 Voltage
    snprintf(buffer, sizeof(buffer), "Cell 2: %d mV", sensor_state->cell2_voltage);
    u8g2_DrawStr(u8g2, 0, 20, buffer);

    // Display Temperature
    snprintf(buffer, sizeof(buffer), "Temp: %d C", sensor_state->cell_temperature);
    u8g2_DrawStr(u8g2, 0, 30, buffer);

    // Display Charge Current
    snprintf(buffer, sizeof(buffer), "Charge: %d mA", sensor_state->charge_rate);
    u8g2_DrawStr(u8g2, 0, 40, buffer);

    // Send the buffer to the display
    u8g2_SendBuffer(u8g2);
}


uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    static uint8_t buffer[32];
    static uint8_t buffer_index;
    uint8_t *data;
    
    switch (msg) {
        case U8X8_MSG_BYTE_INIT:
            TWI_initHost();    // Initialize TWI peripheral
            TWI_initPins();    // Configure TWI pins
            break;
        
        case U8X8_MSG_BYTE_START_TRANSFER:
            buffer_index = 0;
            break;
            
        case U8X8_MSG_BYTE_SEND:
            data = (uint8_t *)arg_ptr;
            while (arg_int > 0) {
                buffer[buffer_index++] = *data++;
                arg_int--;
                if (buffer_index >= sizeof(buffer)) {
                    // Send buffer if full
                    if (!TWI_sendBytes(u8x8_GetI2CAddress(u8x8) >> 1, buffer, buffer_index)) {
                        // Handle error: TWI communication failed
                        return 0;
                    }
                    buffer_index = 0;
                }
            }
            break;
            
        case U8X8_MSG_BYTE_END_TRANSFER:
            if (buffer_index > 0) {
                // Send any remaining data
                if (!TWI_sendBytes(u8x8_GetI2CAddress(u8x8) >> 1, buffer, buffer_index)) {
                    // Handle error: TWI communication failed
                    return 0;
                }
                buffer_index = 0;
            }
            break;
            
        case U8X8_MSG_BYTE_SET_DC:
            // Not used for I2C
            break;
            
        default:
            return 0;
    }
    return 1;
}
uint8_t u8x8_avr_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            // Add any GPIO initialization if needed
            break;
        
        case U8X8_MSG_DELAY_10MICRO:
            _delay_us(10);
            break;
        
        case U8X8_MSG_DELAY_100NANO:
            _delay_us(1);  // Smallest practical delay
            break;
        
        case U8X8_MSG_DELAY_MILLI:
            while (arg_int--) {
                _delay_ms(1);
            }
            break;
            
        default:
            return 0;
    }
    return 1;
}
