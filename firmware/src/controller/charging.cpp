#include "hal.h"
#include "charging.h"
#include "gpio.h"
#include "adc.h"
#include <math.h>
#include <Adafruit_INA260.h>
#include "debug.h"

#define K 1  // Constant for scaling.

static charging_state_t charging_state;

static Adafruit_INA260 battery_ina = Adafruit_INA260();
static Adafruit_INA260 supply_ina = Adafruit_INA260();

void charging_init() 
{
    if (!battery_ina.begin(BATTERY_INA_ADDRESS)) {
        D_printlnf("Couldn't find battery INA260");
        while (1);
    }
    if (!supply_ina.begin(SUPPLY_INA_ADDRESS)) {
        D_printlnf("Couldn't find supply INA260");
        while (1);
    }
}

charging_state_t charging_get_state()
{
    return charging_state;
}

void charging_update_state()
{
    charging_state.battery_metrics.ina.bus_voltage_v = battery_ina.readBusVoltage() / (float)1000;
    charging_state.battery_metrics.ina.current_ma = battery_ina.readCurrent();
    charging_state.battery_metrics.ina.power_w = battery_ina.readPower() / (float)1000;

    charging_state.supply_metrics.ina.bus_voltage_v = supply_ina.readBusVoltage() / (float)1000;
    charging_state.supply_metrics.ina.current_ma = supply_ina.readCurrent();
    charging_state.supply_metrics.ina.power_w = supply_ina.readPower() / (float)1000;

    charging_state.buck_voltage_v = adc_read(CHARGE_VOLTAGE_PIN, CHARGE_VOLTAGE_DIVIDER_RATIO);
}

uint8_t charging_calculate_duty_cycle()
{
    uint8_t current_duty_cycle = charging_state.duty_cycle_uint8;
    float current_charging_voltage_v = charging_state.battery_metrics.ina.bus_voltage_v;
    
    float critical = 1;

    float voltage_overshoot = current_charging_voltage_v - CHARGING_VOLTAGE_V;
    float error = fabs(voltage_overshoot);

    // If we are within the tolerance, keep the same duty cycle
    if (fabsf(voltage_overshoot) < CHARING_VOLTAGE_TOLERANCE) {
        return current_duty_cycle;
    }

    float alpha = 1.0 + (K * log(error / critical));
    if(alpha < 0) {        // clamp alpha to zero, if alpha <0
        alpha = 0;
    }
    float duty_cycle_step_uint8 = pow(error,alpha) + 1.0;

    // Ensure duty cycle step converges to minimum value at certain threshold
    if (error < SLOW_STEP_THESHOLD_V) {
        duty_cycle_step_uint8 = 1;
    }    

    if (voltage_overshoot > 0) {
        // Decrease duty cycle -> decrease voltage
        return constrain(current_duty_cycle - duty_cycle_step_uint8, 0, 255);
    } else {
        // Increase duty cycle -> increase voltage
        return constrain(current_duty_cycle + duty_cycle_step_uint8, 0, 255);
    }
}

void charging_set_duty_cycle(uint8_t duty_cycle)
{
    uint8_t sanitized_duty_cycle = constrain(duty_cycle, 0, 255);
    analogWrite(CHARGE_PWM_PIN, sanitized_duty_cycle);
    charging_state.duty_cycle_uint8 = sanitized_duty_cycle;
}

static void charging_set_shutdown_pin(bool shutdown)
{
    if (shutdown) {
        pinMode(H_BRIDGE_SHUT_DOWN_PIN_AL, OUTPUT);
        hal_digital_write(H_BRIDGE_SHUT_DOWN_PIN_AL, LOW);
    } else {
        hal_pin_mode(H_BRIDGE_SHUT_DOWN_PIN_AL, INPUT_PULLUP);
    }
}

void charging_disable()
{
    // Pull shut-down pin low on gate driver
    charging_set_shutdown_pin(true);

    // As an extra step, set duty cycle to 0
    charging_set_duty_cycle(0);

    return;
}

void charging_enable()
{
    // Pull shut-down pin low on gate driver
    charging_set_shutdown_pin(false);

    return;
}


bool charging_current_within_limits()
{
    bool in_limits = (charging_state.battery_metrics.ina.current_ma < MAX_CHARGE_CURRENT_mA);
    return in_limits;
}

float charging_get_power_efficiency()
{
    float percent = (charging_state.battery_metrics.ina.power_w / charging_state.supply_metrics.ina.power_w) * 100;
    return percent;
}
