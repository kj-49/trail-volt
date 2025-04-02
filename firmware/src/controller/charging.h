
#ifndef CHARGING_H
#define	CHARGING_H

#include "mode.h"
#include <stdint.h>

// App Voltage = ADC reading * CHARGE_VOLTAGE_DIVIDER_RATIO
const float CHARGE_VOLTAGE_DIVIDER_RATIO = 3.925;

/**
 * The acceptable difference between CHARGING_VOLTAGE_V and our current charging.
 */
const float CHARING_VOLTAGE_TOLERANCE = 0.050;

/*
 * At this error value, set the pwm step to minimum.
 */
const float SLOW_STEP_THESHOLD_V = 1.00;

const float CHARGING_VOLTAGE_V = 8.2;

const float MAX_CHARGE_CURRENT_mA = 500.0;

// A0 & A1 tied to GND
#define SUPPLY_INA_ADDRESS 0x45
// A0 & A1 tied to VSS
#define BATTERY_INA_ADDRESS 0x40

typedef struct {
  float current_ma;
  float bus_voltage_v;
  float power_w;
} ina_metrics_t;

/*
 * Wrap the types for better type safety throughout the code.
 */
typedef struct {
    ina_metrics_t ina;
} battery_metrics_t;

typedef struct {
    ina_metrics_t ina;
} supply_metrics_t;

typedef struct {
  uint8_t duty_cycle_uint8;
  float buck_voltage_v;
  battery_metrics_t battery_metrics;
  supply_metrics_t supply_metrics;
} charging_state_t;

/**
 * @brief  Initialize charging.
 */
void charging_init();

/**
 * @brief  Get the charging state.
 * @return The charging state.
 */
charging_state_t charging_get_state();

/**
 * @brief  Updates the charging state.
 */
void charging_update_state();

/**
 * @brief  Adjusts the duty cycle based on the charging status provided.
 * @param  duty_cycle: The duty cycle.
 */
void charging_set_duty_cycle(uint8_t duty_cycle);

/**
 * @brief  Sets the shutdown pin
 */
void charging_stop();

/**
 * @brief  Calculates the new duty cycle applied to gate driver input.
 * @return The new calculated PWM duty cycle.
 */
uint8_t charging_calculate_duty_cycle();

/**
 * @brief  Determines whether the charging current is within the allowable limits.
 * @return True if within the allowable limits.
 */
bool charging_current_within_limits();

/**
 * @brief  Calculates the power efficiency of our charging circuit.
 * @return The power efficiency as a percentage.
 */
float charging_get_power_efficiency();

#endif	/* CHARGING_H */



