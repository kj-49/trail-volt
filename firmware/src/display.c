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

void draw_logo(u8g2_t *u8g2){
    static const unsigned char logo_bits[] U8X8_PROGMEM = {
        // '3', 128x40px
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc7, 0x01, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 
        0x00, 0x78, 0x00, 0xe0, 0xff, 0x01, 0x00, 0x00, 0xc7, 0x01, 0x0e, 0x38, 0x00, 0xe0, 0x00, 0x00, 
        0x00, 0x7c, 0x00, 0xe0, 0xff, 0x01, 0x00, 0x00, 0xc7, 0x01, 0x0e, 0x38, 0x00, 0xe0, 0x60, 0x00, 
        0x00, 0x7c, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0xc0, 0x01, 0x0e, 0x1c, 0x00, 0xe0, 0x60, 0x00, 
        0x00, 0x7e, 0x00, 0x00, 0x0e, 0x00, 0x80, 0x01, 0xc0, 0x01, 0x1c, 0x1c, 0x30, 0xe0, 0xf0, 0x00, 
        0x00, 0x7e, 0x00, 0x00, 0x0e, 0xf8, 0xf3, 0x0f, 0xc7, 0x01, 0x1c, 0x1c, 0xfc, 0xe1, 0xf8, 0x07, 
        0x00, 0x7f, 0x00, 0x00, 0x0e, 0xf8, 0xf3, 0x0f, 0xc7, 0x01, 0x1c, 0x0e, 0xfe, 0xe3, 0xf8, 0x07, 
        0x00, 0xff, 0x1f, 0x00, 0x0e, 0x78, 0x30, 0x0c, 0xc7, 0x01, 0x38, 0x0e, 0x87, 0xe3, 0xe0, 0x00, 
        0x80, 0xff, 0x1f, 0x00, 0x0e, 0x38, 0x00, 0x0e, 0xc7, 0x01, 0x38, 0x0e, 0x87, 0xe7, 0x60, 0x00, 
        0x80, 0xff, 0x0f, 0x00, 0x0e, 0x38, 0xc0, 0x0f, 0xc7, 0x01, 0x38, 0x07, 0x07, 0xe7, 0x60, 0x00, 
        0xc0, 0xff, 0x0f, 0x00, 0x0e, 0x38, 0xf0, 0x0f, 0xc7, 0x01, 0x70, 0x87, 0x03, 0xe7, 0x60, 0x00, 
        0xc0, 0xff, 0x07, 0x00, 0x0e, 0x38, 0x78, 0x0c, 0xc7, 0x01, 0x70, 0x07, 0x07, 0xe7, 0x60, 0x00, 
        0xe0, 0xff, 0x07, 0x00, 0x0e, 0x38, 0x38, 0x0c, 0xc7, 0x01, 0xf0, 0x03, 0x87, 0xe3, 0xe0, 0x00, 
        0xe0, 0xff, 0x03, 0x00, 0x0e, 0x38, 0x38, 0x1e, 0xc7, 0x01, 0xe0, 0x03, 0xcf, 0xe3, 0xe1, 0x00, 
        0x00, 0xf8, 0x03, 0x00, 0x0e, 0x38, 0xf8, 0x3f, 0xc7, 0x07, 0xe0, 0x03, 0xfe, 0xc1, 0xe3, 0x07, 
        0x00, 0xf0, 0x01, 0x00, 0x0e, 0x38, 0xf0, 0x39, 0x87, 0x07, 0xe0, 0x01, 0xfc, 0xc0, 0xc3, 0x07, 
        0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    u8g2_ClearBuffer(u8g2);

    u8g2_DrawXBM(u8g2, 0, 12, 128, 40, logo_bits);

    u8g2_SendBuffer(u8g2);
}

void configure_display(u8g2_t *u8g2) {

    // Initialize display (SH1106 128x64)
    u8g2_Setup_sh1106_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_avr_gpio_and_delay);
    
    // Set I2C address for the SH1106 display (0x3C or 0x3D depending on the display)
    u8g2_SetI2CAddress(u8g2, 0x3C << 1);  // Shift left to match 7-bit address format
    u8g2_InitDisplay(u8g2);
    u8g2_SetPowerSave(u8g2, 0);  // Wake up the display
    
    u8g2_SetFont(u8g2, u8g2_font_profont10_tr);
}

void update_display(u8g2_t *u8g2, const sensor_state_t *sensor_state, volatile const application_state_t *application_state) {
    // Clear the display buffer
    u8g2_ClearBuffer(u8g2);
    
    // SH1106 128x64 display dimensions
    const uint8_t disp_width = 128;
    const uint8_t disp_height = 64;
    
    // Battery dimensions and positions
    uint8_t battery_width = 16;
    uint8_t battery_height = 35;
    uint8_t terminal_width = 6;
    uint8_t terminal_height = 3;
    uint8_t battery_spacing = 8;
    uint8_t start_x = 5;
    uint8_t start_y = 15; // Moved down slightly to fit temperature text above
    
    // Text area for sensor values
    uint8_t text_start_x = start_x + 2 * battery_width + battery_spacing + 8;
    uint8_t text_start_y = 12;
    uint8_t line_height = 10;
    
    // Maximum battery voltage
    float max_voltage = 4.2;
    
    // Draw Battery 1
    u8g2_DrawFrame(u8g2, start_x, start_y + terminal_height, battery_width, battery_height);
    u8g2_DrawBox(u8g2, start_x + (battery_width - terminal_width) / 2, start_y, terminal_width, terminal_height);
    
    // Calculate fill level for Battery 1
    float volt1 = sensor_state->cell_1_voltage_mv / 1000.0;
    uint8_t fill_height1 = (volt1 * battery_height) / max_voltage;
    if (fill_height1 > battery_height) fill_height1 = battery_height;
    
    // Draw fill level
    u8g2_DrawBox(u8g2, 
                start_x + 2, 
                start_y + terminal_height + battery_height - fill_height1, 
                battery_width - 4, 
                fill_height1);
    
    // Draw Battery 2
    uint8_t batt2_x = start_x + battery_width + battery_spacing;
    u8g2_DrawFrame(u8g2, batt2_x, start_y + terminal_height, battery_width, battery_height);
    u8g2_DrawBox(u8g2, batt2_x + (battery_width - terminal_width) / 2, start_y, terminal_width, terminal_height);
    
    // Calculate fill level for Battery 2
    float volt2 = sensor_state->cell_2_voltage_mv / 1000.0;
    uint8_t fill_height2 = (volt2 * battery_height) / max_voltage;
    if (fill_height2 > battery_height) fill_height2 = battery_height;
    
    // Draw fill level
    u8g2_DrawBox(u8g2, 
                batt2_x + 2, 
                start_y + terminal_height + battery_height - fill_height2, 
                battery_width - 4, 
                fill_height2);
    
    // Display temperature readings above respective batteries
    char str_buf[16];

    // Battery 1 temperature above first battery
    snprintf(str_buf, sizeof(str_buf), "%dC", sensor_state->cell_1_temperature_c);
    uint8_t temp1_width = u8g2_GetStrWidth(u8g2, str_buf);
    u8g2_DrawStr(u8g2, 
                start_x + (battery_width - temp1_width) / 2, 
                start_y - 5,  // Position above the battery
                str_buf);

    // Battery 2 temperature above second battery
    snprintf(str_buf, sizeof(str_buf), "%dC", sensor_state->cell_2_temperature_c);
    uint8_t temp2_width = u8g2_GetStrWidth(u8g2, str_buf);
    u8g2_DrawStr(u8g2, 
                batt2_x + (battery_width - temp2_width) / 2, 
                start_y - 5,  // Position above the battery
                str_buf);
    
    // Display voltage readings under respective batteries
    // Battery 1 voltage
    snprintf(str_buf, sizeof(str_buf), "%.2f", volt1);
    uint8_t volt1_width = u8g2_GetStrWidth(u8g2, str_buf);
    u8g2_DrawStr(u8g2, 
                start_x + (battery_width - volt1_width) / 2, 
                start_y + terminal_height + battery_height + 10, 
                str_buf);
    
    // Battery 2 voltage
    snprintf(str_buf, sizeof(str_buf), "%.2f", volt2);
    uint8_t volt2_width = u8g2_GetStrWidth(u8g2, str_buf);
    u8g2_DrawStr(u8g2, 
                batt2_x + (battery_width - volt2_width) / 2, 
                start_y + terminal_height + battery_height + 10, 
                str_buf);
    
    // Display other sensor values on the right side
    uint8_t y_pos = text_start_y;
    
    switch(application_state->mode) {
            case STATE_MONITORING:
                snprintf(str_buf, sizeof(str_buf), "MONITORING");
                break;
            case STATE_CHARGING:
                snprintf(str_buf, sizeof(str_buf), "CHARGING");
                break;
            case STATE_SLEEP:
                snprintf(str_buf, sizeof(str_buf), "SLEEPING");
            default:
                break;
        }
     
    u8g2_SetFont(u8g2, u8g2_font_prospero_bold_nbp_tf);
    u8g2_DrawStr(u8g2, text_start_x, y_pos, str_buf);
    u8g2_SetFont(u8g2, u8g2_font_profont10_tr);
    y_pos += line_height;
    
    // Total voltage
    float avg_volt = (volt1 + volt2) / 2;
    snprintf(str_buf, sizeof(str_buf), "Avg: %.2fV", avg_volt);
    u8g2_DrawStr(u8g2, text_start_x, y_pos, str_buf);
    y_pos += line_height;
    
    snprintf(str_buf, sizeof(str_buf), "Curr: %dmA", sensor_state->charge_rate_ma);
    u8g2_DrawStr(u8g2, text_start_x, y_pos, str_buf);
    y_pos += line_height;
    
    snprintf(str_buf, sizeof(str_buf), "PWM: %d%", application_state->current_duty_cycle);
    u8g2_DrawStr(u8g2, text_start_x, y_pos, str_buf);
    y_pos += line_height;
    
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
