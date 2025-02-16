/* 
 * File:   sensors.c
 * Author: Kyle James
 *
 * Created on February 15, 2025, 9:29 PM
 */

#include "sensors.h"


void update_sensor_state(sensor_state_t *current_state)
{
    adc_result_t cell_1_result;

    ADC0_ConversionStart();
    while (!ADC0_IsConversionDone());
    cell_1_result = ADC0_ConversionResultGet();
    ADC0_ConversionDoneInterruptFlagClear();

    // Divide by the amount of reading averaged
    current_state->cell_1_voltage_mv = cell_1_result / ADC_ACCUMULATION;
}
    


