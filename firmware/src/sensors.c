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
#include "configuration.h"

uint16_t adc_to_mv(uint16_t adc_value, float voltage_divider_ratio);
uint16_t read_from_adc(adc_channel_t channel, float voltage_divider_ratio);

void update_sensor_state(sensor_state_t *sensor_state)
{
    uint16_t value;
    
    /* ADC0 */
    value = read_from_adc(ADC0_CHANNEL_AIN0, CELL_VOLTAGE_DIVIDER_RATIO);
    sensor_state->cell_1_voltage_mv = value;
    
    /* ADC1 */
    value = read_from_adc(ADC0_CHANNEL_AIN1, CELL_VOLTAGE_DIVIDER_RATIO);
    sensor_state->cell_2_voltage_mv = value;
    
    /* ADC2 */
    value = read_from_adc(ADC0_CHANNEL_AIN2, 1);
    sensor_state->cell_1_temperature_c = value;
    
    /* ADC3 */
    value = read_from_adc(ADC0_CHANNEL_AIN3, 1);
    sensor_state->cell_2_temperature_c = value;
    
    /* ADC4 */
    value = read_from_adc(ADC0_CHANNEL_AIN4, 1);
    sensor_state->charge_rate_ma = value;
    
    // Set valid
    sensor_state->values_valid = true;
}

/**
 * @brief  Reads value from ADC0.
 * @param  channel: The pin to read from.
 * @param  voltage_divider_ratio: The ratio by which the voltage is divided (e.g., 1/3 for a 3:1 divider).
 * @return ADC reading in millivolts.
 */
uint16_t read_from_adc(adc_channel_t channel, float voltage_divider_ratio)
{
    ADC0_select_channel(channel);
    //_delay_us(100);
    ADC0_begin_conversion();
    
    while(!ADC0_conversion_is_done()){};
    uint16_t result = ADC0_get_conversion_result();
    
    ADC0_clear_conversion_isr_flag();
    
    return adc_to_mv(result, voltage_divider_ratio);
}

/**
 * @brief  Converts an ADC value to millivolts.
 * @param  adc_value: Raw ADC reading (0 to ADC max resolution).
 * @param  voltage_divider_ratio: The ratio by which the voltage is divided (e.g., 1/3 for a 3:1 divider).
 * @return Converted voltage in millivolts.
 * 
 * This function converts a raw ADC reading to millivolts using the formula:
 * 
 *    V_measured (mV) = (adc_value / ADC_MAX) * VREF * 1000
 * 
 */
uint16_t adc_to_mv(uint16_t adc_value, float voltage_divider_ratio)
{
    int16_t voltage_equivalent = (adc_value / ADC_ACCUMULATION) * ADC_VREF_VALUE * 1000 / 4096;

    int16_t ratio_adjusted = voltage_equivalent / voltage_divider_ratio;
    
    return ratio_adjusted;
}
    


