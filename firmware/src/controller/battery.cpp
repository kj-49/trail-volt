#include "battery.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>
#include "temperature.h"

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

