#include "temperature.h"
#include "adc.h"

#include <math.h>
#include <stdint.h>

/**
 * @brief  Gets the resistance by reading the voltage drop across the thermistor.
 * @return The resistance in Ohms.
 */
float get_resistance(int pin)
{
    float voltage = adc_read(pin, THERMISTOR_ADC_DIVISION);

    // Thermistors are on low-side of dividers
    float resistance = (voltage * SERIES_RESISTOR) / (ADC_VREF_CALIBRATED - voltage);

    return resistance;
}

/**
 * @brief  Gets the temperature assuming a thermistor is attached to the given pin.
 * @param  pin: The ADC pin attached to the thermistor.
 * @return The temperature in C.
 */
static float get_temp_from_thermistor(int pin)
{
    const float R0 = 10000.0;
    const float Beta = 3380.0;
    const float T0 = 298.15;
    
    float r_ntc = get_resistance(pin);

    // Use a Steinhart equation for conversions.
    float steinhart = (1.0f / T0) + (1.0f / Beta) * log(r_ntc / R0);
    float t_kelvin = 1.0f / steinhart;
    float t_celsius = t_kelvin - 273.15f;
    
    return t_celsius;
}

float temperature_get_series_reading()
{
    float temp_c = get_temp_from_thermistor(THERMISTOR_2_PIN);
    return temp_c;
}

float temperature_get_ground_reading()
{
    float temp_c = get_temp_from_thermistor(THERMISTOR_1_PIN);
    return temp_c;
}




