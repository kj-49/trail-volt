/* 
 * File:   sensors.h
 * Author: Kyle James
 *
 * Created on February 15, 2025, 6:43 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H

#include <stdint.h>

typedef struct {
    uint16_t cell_1_voltage_mv;
    uint16_t cell_2_voltage_mv;
    uint16_t cell_1_temperature_c;
    uint16_t cell_2_temperature_c;
    uint16_t charge_rate_ma;
    uint8_t values_valid;
} sensor_state_t;

#define ADC_ACCUMULATION 4

#define CELL_VOLTAGE_DIVIDER_RATIO 0.5

/**
 * @brief  Updates sensor state based on values taken from ADC0.
 * @param  sensor_state: A pointer to the application sensor state.
 */
void update_sensor_state(sensor_state_t *sensor_state);

#endif	/* SENSORS_H */

