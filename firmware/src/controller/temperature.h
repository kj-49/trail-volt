#ifndef TEMPERATURE_H
#define	TEMPERATURE_H

#define IS_HIGH_SIDE 1

const float SERIES_RESISTOR = 10000.0;

#define THERMISTOR_ADC_DIVISION 1

#include "gpio.h"

/**
 * @brief  Gets the temperature from the thermistor attached the common series connection.
 * @return The temperature in C.
 */
float temperature_get_series_reading();

/**
 * @brief  Gets the temperature from the thermistor attached the common ground connection.
 */
float temperature_get_ground_reading();

#endif /* TEMPERATURE_H */
