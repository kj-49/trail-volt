#define F_CPU 24000000UL

#include <avr/io.h>
#include <stdbool.h>

#include "sensors.h"
#include "display.h"


int main(void) {
    
    // Initialize state with invalid flag true
    sensor_state_t sensor_state = {2450, 3320, 23, 200, 0};


    u8g2_t u8g2;
    
    configure_display(&u8g2);
    
    while (1) {
        update_display(&u8g2, &sensor_state);
    }
    
    return 0;
}