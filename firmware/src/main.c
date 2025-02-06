#include <avr/io.h>
#include <util/delay.h>
#include "../lib/u8g2.X/u8g2.h"
#include "../lib/twi.X/twi-host.X/peripherals/TWI/TWI_host.h"

#define F_CPU 24000000UL

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

int main(void) {
    
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSCHF_gc;  // Select internal 16 MHz oscillator

    // Set the prescaler to 1 (no division)
    CLKCTRL.MCLKCTRLB = 0;  // No prescaling (no division)
    
    u8g2_t u8g2;
    
    // Initialize display (SH1106 128x64)
    u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_avr_gpio_and_delay);
    
    
    // Set I2C address for the SH1106 display (0x3C or 0x3D depending on the display)
    u8g2_SetI2CAddress(&u8g2, 0x3C << 1);  // Shift left to match 7-bit address format
    u8g2_InitDisplay(&u8g2);
    _delay_ms(10000);
    
    u8g2_SetPowerSave(&u8g2, 0);  // Wake up the display
    //u8g2_SetContrast(&u8g2, 128);  // Optional: Set contrast
    
    while (1) {
        u8g2_ClearBuffer(&u8g2);
        
        // Draw something
        u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
        u8g2_DrawStr(&u8g2, 0, 10, "Hello, World!");
        
        //u8g2_SendBuffer(&u8g2);
        //_delay_ms(10);

        u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
        u8g2_DrawStr(&u8g2, 0, 10, "Hello, Worder!");
        u8g2_SendBuffer(&u8g2);
        //_delay_ms(100);
    }
    
    return 0;
}