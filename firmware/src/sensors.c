/* 
 * File:   sensors.c
 * Author: Kyle James
 *
 * Created on February 15, 2025, 9:29 PM
 */

#include "sensors.h"
#include "adc.h"
#include <stdbool.h>
#include <avr/delay.h>

uint16_t convert_from_adc(uint16_t adc_value);
uint16_t read_from_adc(adc_channel_t channel);

void update_sensor_state(sensor_state_t *current_state)
{
    uint16_t value;
    
    /* ADC0 */
    value = read_from_adc(ADC0_CHANNEL_AIN0);
    current_state->cell_1_voltage_mv = value;
    
    /* ADC1 */
    value = read_from_adc(ADC0_CHANNEL_AIN1);
    current_state->cell_2_voltage_mv = value;
    
    /* ADC2 */
    value = read_from_adc(ADC0_CHANNEL_AIN2);
    current_state->cell_temperature_c = value;
    
    /* ADC3 */
    value = read_from_adc(ADC0_CHANNEL_AIN3);
    current_state->charge_rate_ma = value;
    
    // Set valid
    current_state->values_valid = true;
}

uint16_t read_from_adc(adc_channel_t channel)
{
    ADC0_select_channel(channel);
    _delay_us(10);
    ADC0_begin_conversion();
    
    while(!ADC0_conversion_is_done()){};
    uint16_t result = ADC0_get_conversion_result();
    
    ADC0_clear_conversion_isr_flag();
    
    return convert_from_adc(result);
}

uint16_t convert_from_adc(uint16_t adc_value)
{
    return adc_value / ADC_ACCUMULATION;
}
    


