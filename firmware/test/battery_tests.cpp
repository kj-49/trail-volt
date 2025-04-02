#include "unity/unity.h"
#include "fff.h"
#include "../src/controller/battery.h"
#include "../src/controller/hal.h"
#include "../src/controller/gpio.h"

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(hal_digital_write, int, int);

FAKE_VALUE_FUNC(float, adc_read, int, float);
FAKE_VALUE_FUNC(float, temperature_get_series_reading);
FAKE_VALUE_FUNC(float, temperature_get_ground_reading);

void setUp(void)
{
    RESET_FAKE(hal_digital_write);
    RESET_FAKE(adc_read);
    RESET_FAKE(temperature_get_series_reading);
    RESET_FAKE(temperature_get_ground_reading);
    
    adc_read_fake.return_val = 3.3f; 
    temperature_get_series_reading_fake.return_val = 25.0f;
    temperature_get_ground_reading_fake.return_val = 23.0f;
    
    // Reset battery state to known values before each test
    battery_state_t default_state = {};
    default_state.upper_cell_voltage_v = 3.7f;
    default_state.lower_cell_voltage_v = 3.7f;
    default_state.series_temperature_c = 25.0f;
    default_state.ground_temperature_c = 23.0f;
    default_state.upper_discharging = false;
    default_state.lower_discharging = false;
    
    // Set the battery state to our default values
    battery_set_state(default_state);
}

void tearDown(void)
{
}

void test_set_upper_discharge(void)
{
    // Test enabling upper discharge
    battery_set_upper_discharge(true);
    TEST_ASSERT_EQUAL(1, hal_digital_write_fake.call_count);
    TEST_ASSERT_EQUAL(C2_BALANCING_PIN, hal_digital_write_fake.arg0_val);
    TEST_ASSERT_EQUAL(HIGH, hal_digital_write_fake.arg1_val);
    TEST_ASSERT_TRUE(battery_get_state().upper_discharging);

    // Reset fake function call count
    RESET_FAKE(hal_digital_write);

    // Test disabling upper discharge
    battery_set_upper_discharge(false);
    TEST_ASSERT_EQUAL(1, hal_digital_write_fake.call_count);
    TEST_ASSERT_EQUAL(C2_BALANCING_PIN, hal_digital_write_fake.arg0_val);
    TEST_ASSERT_EQUAL(LOW, hal_digital_write_fake.arg1_val);
    TEST_ASSERT_FALSE(battery_get_state().upper_discharging);
}

void test_set_lower_discharge(void)
{
    // Test enabling lower discharge
    battery_set_lower_discharge(true);
    TEST_ASSERT_EQUAL(1, hal_digital_write_fake.call_count);
    TEST_ASSERT_EQUAL(C1_BALANCING_PIN, hal_digital_write_fake.arg0_val);
    TEST_ASSERT_EQUAL(HIGH, hal_digital_write_fake.arg1_val);
    TEST_ASSERT_TRUE(battery_get_state().lower_discharging);

    // Reset fake function call count
    RESET_FAKE(hal_digital_write);

    // Test disabling lower discharge
    battery_set_lower_discharge(false);
    TEST_ASSERT_EQUAL(1, hal_digital_write_fake.call_count);
    TEST_ASSERT_EQUAL(C1_BALANCING_PIN, hal_digital_write_fake.arg0_val);
    TEST_ASSERT_EQUAL(LOW, hal_digital_write_fake.arg1_val);
    TEST_ASSERT_FALSE(battery_get_state().lower_discharging);
}

void test_battery_is_fully_charged(void)
{
    battery_metrics_t metrics;
    metrics.ina.bus_voltage_v = 7.0f; // Below fully charged threshold for combined cells
    
    // Both cells below threshold
    battery_state_t normal_state = {};
    normal_state.upper_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V - 0.1f;
    normal_state.lower_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V - 0.1f;
    battery_set_state(normal_state);
    TEST_ASSERT_FALSE(battery_is_fully_charged(metrics));
    
    // Upper cell above threshold
    battery_state_t upper_charged_state = {};
    upper_charged_state.upper_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V + 0.1f;
    upper_charged_state.lower_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V - 0.1f;
    battery_set_state(upper_charged_state);
    TEST_ASSERT_TRUE(battery_is_fully_charged(metrics));
    
    // Lower cell above threshold
    battery_state_t lower_charged_state = {};
    lower_charged_state.upper_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V - 0.1f,
    lower_charged_state.lower_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V + 0.1f,
    battery_set_state(lower_charged_state);
    TEST_ASSERT_TRUE(battery_is_fully_charged(metrics));
    
    // Both cells above threshold
    battery_state_t both_charged_state = {};
    both_charged_state.upper_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V + 0.1f;
    both_charged_state.lower_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V + 0.1f;
    battery_set_state(both_charged_state);
    TEST_ASSERT_TRUE(battery_is_fully_charged(metrics));
    
    // Combined voltage check (both cells below threshold but INA voltage high)
    battery_state_t normal_voltage_state = {};
    normal_voltage_state.upper_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V - 0.1f;
    normal_voltage_state.lower_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V - 0.1f;
    battery_set_state(normal_voltage_state);
    metrics.ina.bus_voltage_v = FULLY_CHARGED_SINGLE_CELL_V * 2.0f + 0.1f;
    TEST_ASSERT_TRUE(battery_is_fully_charged(metrics));
    
    // All measurements below threshold
    battery_state_t all_normal_state = {};
    all_normal_state.upper_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V - 0.1f,
    all_normal_state.lower_cell_voltage_v = FULLY_CHARGED_SINGLE_CELL_V - 0.1f,
    battery_set_state(all_normal_state);
    metrics.ina.bus_voltage_v = FULLY_CHARGED_SINGLE_CELL_V * 2.0f - 0.1f;
    TEST_ASSERT_FALSE(battery_is_fully_charged(metrics));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_set_upper_discharge);
    RUN_TEST(test_set_lower_discharge);
    RUN_TEST(test_battery_is_fully_charged);
    return UNITY_END();
}