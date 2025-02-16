/* 
 * File:   sensors.h
 * Author: Kyle James
 *
 * Created on February 15, 2025, 6:43 PM
 */

#ifndef SENSORS_H
#define	SENSORS_H

typedef struct {
    uint16_t cell_1_voltage_mv;
    uint16_t cell_2_voltage_mv;
    uint16_t cell_temperature_c;
    uint16_t charge_rate_ma;
    uint8_t values_valid;
} sensor_state_t;

#endif	/* SENSORS_H */

