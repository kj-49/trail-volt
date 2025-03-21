#include "temperature.h"
#include "adc.h"

#include <math.h>
#include <stdint.h>

/**
 * @brief  Gets the resistance by reading the voltage drop across the thermistor.
 * @return The resistance in Ohms.
 */
static float get_resistance(int pin) {
    float voltage = read_from_adc(pin, THERMISTOR_ADC_DIVISION);

    float drop;
    if (IS_HIGH_SIDE) {
        drop = 5.0f - voltage;
    } else {
        drop = voltage;
    }

    float resistance = (5.0f * SERIES_RESISTOR) / drop;
}

/**
 * @brief  Gets the temperature assuming a thermistor is attached to the given pin.
 * @param  pin: The ADC pin attached to the thermistor.
 * @return The temperature in C.
 */
static float get_temp_from_thermistor(int pin) {
    const float R0 = 10000.0f;
    const float R_fixed = 40000.0f;
    const float Beta = 3380.0f;            // These were found in datasheet.
    const float T0 = 298.15f;
    
    float r_ntc = get_resistance(pin);

    // Use a Steinhart equation for conversions.
    float steinhart = (1.0f / T0) + (1.0f / Beta) * log(r_ntc / R0);
    float t_kelvin = 1.0f / steinhart;
    float t_celsius = t_kelvin - 273.15f;
    
    return t_celsius;
}

float temperature_get_series_reading() {
    float temp_c = get_temp_from_thermistor(THERMISTOR_2_PIN);
    return temp_c;
}

float temperature_get_ground_reading() {
    float temp_c = get_temp_from_thermistor(THERMISTOR_1_PIN);
    return temp_c;
}




