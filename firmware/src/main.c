
#define F_CPU 4000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <stdbool.h>

#include "sensors.h"
#include "display.h"
#include "adc.h"
#include "state.h"
#include "power.h"

void configure();

/*
 * Need global state so that interrupts have access.
 * Volatile is neccessary due to changes made by interrupts that are not known to compiler.
 */
volatile application_state_t application_state = STATE_MONITORING;

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
        
        switch(application_state) {
            case STATE_MONITORING:
                /* Start sleep timer */
                check_update_state(&application_state);
                
                // Take readings
                update_sensor_state(&sensor_state);
                // Update display
                update_display(&u8g2, &sensor_state, &application_state);
                
                /* Check sleep timer */
                /* If timer passed threshold, enter sleep state.*/
                
                break;
            case STATE_CHARGING:
                while (is_charging()) {
                    
                }
                application_state = STATE_MONITORING;
                break;
            case STATE_SLEEP:
                /* Do nothing */
                break;
            default:
                break;
        }
        

    }
    
    return 0; 
}

void configure() {
    ADC0_configure();
}