
#define F_CPU 4000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <stdbool.h>

#include "sensors.h"
#include "display.h"
#include "adc.h"

void configure();

int main(void) {
    
    configure();
    
    // Initialize state with invalid flag true
    sensor_state_t sensor_state = {0, 0, 0, 0, false};

    u8g2_t u8g2;
    
    configure_display(&u8g2);
    
    // Bootup logo
    draw_logo(&u8g2);
    _delay_ms(2500);
    
    while (1) {
        // Take readings
        update_sensor_state(&sensor_state);
        // Update display
        update_display(&u8g2, &sensor_state);
    }
    
    return 0; 
}

void configure() {
    ADC0_configure();
}