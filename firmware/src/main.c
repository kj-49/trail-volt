
#include <avr/io.h>
#include <stdbool.h>

#include "sensors.h"
#include "display.h"


int main(void) {
    
    // Initialize state with invalid flag true
    sensor_state_t sensor_state = {0, 0, 0, 0, false};

    SYSTEM_Initialize();

    u8g2_t u8g2;
    
    configure_display(&u8g2);
    
    while (1) {
        // Take readings
        update_sensor_state(&sensor_state);
        // Update display
        update_display(&u8g2, &sensor_state);
    }
    
    return 0; 
}