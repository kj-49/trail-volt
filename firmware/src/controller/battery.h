#ifndef BATTERY_H
#define	BATTERY_H

#include "mode.h"
#include "charging.h"

const float TOTAL_CELL_ADC_DIVISION = 4.0;
const float LOWER_CELL_ADC_DIVISION = 4.0;

/**
 * The tolerable difference between the upper and lower cell voltages.
 */
const float BALANCE_THRESHOLD_V = 0.1;
/**
 * The difference when it is acceptable to stop balancing.
 */
const float BALANCING_DONE_THRESHOLD_V = 0.01;

/*
 * This is the single cell voltage we wish to charge our cells to.
 * 18650 lithium-ion cells can be charge to 4.2V for maximum capacity,
 * but charging to 4.0V is safer with the tradeoff of less capacity.
 */
const float FULLY_CHARGED_SINGLE_CELL_V = 4.0;

/*
 * This is minimum voltage our cells should be discharged to.
 */
const float MINIMUM_SINGLE_CELL_V = 2.5;

/*
 * INR-18650-P30B charging temperature range is 0C to 60C. The discharging
 * range is -40C to 60C.
 */
static const float MAX_CHARGING_TEMP_C = 40.0;
static const float MIN_CHARGING_TEMP_C = 0.0;
static const float MAX_DISCHARGING_TEMP_C = 40.0;
static const float MIN_DISCHARGING_TEMP_C = -20.0;

typedef struct {
    float upper_cell_voltage_v;
    float lower_cell_voltage_v;
    float ground_temperature_c;
    float series_temperature_c;
    bool upper_discharging;
    bool lower_discharging;
} battery_state_t;

/**
 * @brief  Gets the battery state.
 * @return The battery state.
 */
battery_state_t battery_get_state();

/**
 * @brief  Sets the battery state.
 */
void battery_set_state(battery_state_t state);

/**
 * @brief  Sets GPIO pin fed to upper cell discharge accordingly.
 * @param  discharge: True to discharge the cell.
 */
void battery_set_upper_discharge(bool discharge);

/**
 * @brief  Sets GPIO pin fed to lower cell discharge accordingly.
 * @param  discharge: True to discharge the cell.
 */
void battery_set_lower_discharge(bool discharge);

/**
 * @brief  Updates the battery state based on values taken from ADC0.
 */
void battery_update_state();

/**
 * @brief  Determines whether cell balancing is needed based on the battery status.
 * @return True if balancing is needed.
 */
bool battery_balancing_needed();

/**
 * @brief  Determines whether cell balancing is done.
 * @return True if balancing is done.
 */
bool battery_balancing_done();

/**
 * @brief  Get the voltage difference between the upper cell voltage and the lower cell voltage.
 * @return The difference between the upper and lower cell voltages.
 */
float battery_get_voltage_difference();

/**
 * @brief  Determines whether the batteries are fully charged.
 * @return True if fully charged.
 */
bool battery_is_fully_charged(battery_metrics_t battery_metrics);

/**
 * @brief  Determines whether the batteries are depleted
 * @return True if battery charge is below depletion threshold.
 */
bool battery_is_depleted(battery_metrics_t battery_metrics);

/**
 * @brief  Determines whether the batteries are within the allowable charging temperature.
 * @return True if in temperature range.
 */
bool battery_in_charge_temp_range();

/**
 * @brief  Determines whether the batteries are within the allowable discharging temperature.
 * @return True if in temperature range.
 */
bool battery_in_discharge_temp_range();

/**
 * @brief  Calculates the upper battery charge percentage.
 * @return The percentage.
 */
float battery_get_upper_percentage();

/**
 * @brief  Calculates the upper battery charge percentage.
 * @return The percentage.
 */
float battery_get_lower_percentage();

/**
 * @brief  Calculates the total series battery charge percentage.
 * @return The percentage.
 */
float battery_get_total_percentage();

#endif	/* BATTERY_H */

