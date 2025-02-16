
#include <avr/io.h>
#include <stdbool.h>

#include "sensors.h"
#include "display.h"
#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/adc/adc0.h"

int main(void) {
    
    // Initialize state with invalid flag true
    sensor_state_t sensor_state = {2450, 3320, 23, 200, 0};

    SYSTEM_Initialize();

    u8g2_t u8g2;
    
    configure_display(&u8g2);
    
    while (1) {
        adc_result_t res;

        ADC0_ConversionStart();
        while (!ADC0_IsConversionDone());
        res = ADC0_ConversionResultGet();
        ADC0_ConversionDoneInterruptFlagClear();
                
        sensor_state.cell_1_voltage_mv = res / 4;
        
        update_display(&u8g2, &sensor_state);
    }
    
    return 0; 
}