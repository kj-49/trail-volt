#include "hal.h"
#include "adc.h"
#include <stdint.h>

#define ADC_SAMPLES 10

/**
 * @brief  Converts an ADC value to millivolts
 * @param  adc_value: Raw ADC reading (0-1023 for Arduino)
 * @param  voltage_divider_ratio: The ratio by which the voltage is divided
 * @return Converted voltage in volts
 */
static float adc_to_v(uint16_t adc_value, float voltage_divider_ratio)
{
    float voltage = (adc_value * (float)ADC_VREF_CALIBRATED) / 1023;
    return voltage * voltage_divider_ratio;
}

float adc_read(int pin, float voltage_divider_ratio)
{
    uint32_t total = 0;

    for (int i = 0; i < ADC_SAMPLES; i++) {
        total += hal_analog_read(pin);
        hal_delay(2);  // Small delay to ensure stable readings
    }

    uint16_t averaged_value = total / ADC_SAMPLES;
    return adc_to_v(averaged_value, voltage_divider_ratio);
}
