#include "battery.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>
#include "temperature.h"
#include <Arduino.h>

static battery_state_t battery_state;

battery_state_t battery_get_state()
{
    return battery_state;
}

void battery_set_upper_discharge(bool discharge)
{
    if (discharge) {
        digitalWrite(C2_BALANCING_PIN, HIGH);
        battery_state.upper_discharging = true;
    } else {
        digitalWrite(C2_BALANCING_PIN, LOW);
        battery_state.upper_discharging = false;
    }
}

void battery_set_lower_discharge(bool discharge)
{
    if (discharge) {
        digitalWrite(C1_BALANCING_PIN, HIGH);
        battery_state.lower_discharging = true;
    } else {
        digitalWrite(C1_BALANCING_PIN, LOW);
        battery_state.lower_discharging = false;
    }
}

void battery_update_state()
{
    float total_voltage = adc_read(TOTAL_CELL_ADC_PIN, TOTAL_CELL_ADC_DIVISION);

    battery_state.lower_cell_voltage_v = adc_read(LOWER_CELL_ADC_PIN, LOWER_CELL_ADC_DIVISION);
    battery_state.upper_cell_voltage_v = total_voltage - battery_state.lower_cell_voltage_v;

    battery_state.series_temperature_c = temperature_get_series_reading();
    battery_state.ground_temperature_c = temperature_get_ground_reading();
}

float battery_get_voltage_difference()
{
    return fabsf(battery_state.upper_cell_voltage_v - battery_state.lower_cell_voltage_v);
}

bool battery_balancing_needed()
{
    float voltage_diff = battery_get_voltage_difference();
    return (voltage_diff > BALANCE_THRESHOLD_V);
}

bool battery_is_fully_charged(battery_metrics_t battery_metrics)
{
    /*
     * We will determine if our cell is fully charged by each individual cell voltage.
     */
    if (battery_state.upper_cell_voltage_v > FULLY_CHARGED_SINGLE_CELL_V || battery_state.lower_cell_voltage_v > FULLY_CHARGED_SINGLE_CELL_V) {
        return true;
    }

    /*
     * As an extra safety precaution, we will also verify our charging parameter don't
     * indicate a fully charged cell.
     */
    float fully_charged_combined = FULLY_CHARGED_SINGLE_CELL_V * 2.0;
    if (battery_metrics.ina.bus_voltage_v > fully_charged_combined) {
        return true;
    }

    return false;
}

bool battery_is_depleted(battery_metrics_t battery_metrics)
{
    bool upper_depleted = battery_state.upper_cell_voltage_v < MINIMUM_SINGLE_CELL_V;
    bool lower_depleted = battery_state.lower_cell_voltage_v < MINIMUM_SINGLE_CELL_V;

    if (upper_depleted || lower_depleted) {
        return true;
    }

    /*
     * As an extra safety precaution, we will also verify that the INA reading
     * from the total charge node is not below MINIMUM_SINGLE_CELL_V * 2.
     */
    if (battery_metrics.ina.bus_voltage_v < MINIMUM_SINGLE_CELL_V * 2) {
      return true;
    }

    return false;
}

bool battery_in_charge_temp_range()
{
    bool ground_in_range = battery_state.ground_temperature_c < MAX_CHARGING_TEMP_C && battery_state.ground_temperature_c > MIN_CHARGING_TEMP_C;
    bool series_in_range = battery_state.series_temperature_c < MAX_CHARGING_TEMP_C && battery_state.series_temperature_c > MIN_CHARGING_TEMP_C;

    return ground_in_range && series_in_range;
}

bool battery_in_discharge_temp_range()
{
    bool ground_in_range = battery_state.ground_temperature_c < MAX_DISCHARGING_TEMP_C && battery_state.ground_temperature_c > MIN_DISCHARGING_TEMP_C;
    bool series_in_range = battery_state.series_temperature_c < MAX_DISCHARGING_TEMP_C && battery_state.series_temperature_c > MIN_DISCHARGING_TEMP_C;

    return ground_in_range && series_in_range;
}

float battery_get_upper_percentage()
{
    float voltage = battery_state.upper_cell_voltage_v;

    /*
    * We will normalize our battery percentage to our desired the max voltage.
    * This may be lower than the batteries actual maximum voltage.
    */
    float percentage = (voltage / FULLY_CHARGED_SINGLE_CELL_V) * 100;

    return percentage;
}

float battery_get_lower_percentage()
{
    float voltage = battery_state.lower_cell_voltage_v;

    /*
    * We will normalize our battery percentage to our desired the max voltage.
    * This may be lower than the batteries actual maximum voltage.
    */
    float percentage = (voltage / FULLY_CHARGED_SINGLE_CELL_V) * 100;

    return percentage;
}

float battery_get_total_percentage()
{
    float total_voltage = battery_state.upper_cell_voltage_v + battery_state.lower_cell_voltage_v;

    float maximum_series_voltage = FULLY_CHARGED_SINGLE_CELL_V * 2;

    /*
    * We will normalize our battery percentage to our desired the max voltage.
    * This may be lower than the batteries actual maximum voltage.
    */
    float percentage = (total_voltage / maximum_series_voltage) * 100;

    return percentage;
}
