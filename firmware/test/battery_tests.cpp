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

void test_battery_is_depleted(void)
{
    battery_metrics_t metrics;
    metrics.ina.bus_voltage_v = 6.0f; // Default value (MINIMUM_SINGLE_CELL_V * 2 is typically 6.0f)

    // Both cells above minimum, INA voltage above minimum - not depleted
    battery_state_t normal_state = {};
    normal_state.upper_cell_voltage_v = MINIMUM_SINGLE_CELL_V + 0.1f;
    normal_state.lower_cell_voltage_v = MINIMUM_SINGLE_CELL_V + 0.1f;
    battery_set_state(normal_state);
    metrics.ina.bus_voltage_v = (MINIMUM_SINGLE_CELL_V * 2) + 0.1f;
    TEST_ASSERT_FALSE(battery_is_depleted(metrics));

    // Upper cell depleted
    battery_state_t upper_depleted_state = {};
    upper_depleted_state.upper_cell_voltage_v = MINIMUM_SINGLE_CELL_V - 0.1f;
    upper_depleted_state.lower_cell_voltage_v = MINIMUM_SINGLE_CELL_V + 0.1f;
    battery_set_state(upper_depleted_state);
    TEST_ASSERT_TRUE(battery_is_depleted(metrics));

    // Lower cell depleted
    battery_state_t lower_depleted_state = {};
    lower_depleted_state.upper_cell_voltage_v = MINIMUM_SINGLE_CELL_V + 0.1f;
    lower_depleted_state.lower_cell_voltage_v = MINIMUM_SINGLE_CELL_V - 0.1f;
    battery_set_state(lower_depleted_state);
    TEST_ASSERT_TRUE(battery_is_depleted(metrics));

    // Both cells depleted
    battery_state_t both_depleted_state = {};
    both_depleted_state.upper_cell_voltage_v = MINIMUM_SINGLE_CELL_V - 0.1f;
    both_depleted_state.lower_cell_voltage_v = MINIMUM_SINGLE_CELL_V - 0.1f;
    battery_set_state(both_depleted_state);
    TEST_ASSERT_TRUE(battery_is_depleted(metrics));

    // Cells OK but INA voltage below threshold
    battery_state_t cells_ok_state = {};
    cells_ok_state.upper_cell_voltage_v = MINIMUM_SINGLE_CELL_V + 0.1f;
    cells_ok_state.lower_cell_voltage_v = MINIMUM_SINGLE_CELL_V + 0.1f;
    battery_set_state(cells_ok_state);
    metrics.ina.bus_voltage_v = (MINIMUM_SINGLE_CELL_V * 2) - 0.1f;
    TEST_ASSERT_TRUE(battery_is_depleted(metrics));

    // Edge case - exactly at minimum voltage
    battery_state_t edge_case_state = {};
    edge_case_state.upper_cell_voltage_v = MINIMUM_SINGLE_CELL_V;
    edge_case_state.lower_cell_voltage_v = MINIMUM_SINGLE_CELL_V;
    battery_set_state(edge_case_state);
    metrics.ina.bus_voltage_v = MINIMUM_SINGLE_CELL_V * 2;
    TEST_ASSERT_FALSE(battery_is_depleted(metrics));
}

void test_battery_in_charge_temp_range(void)
{
    // Test case 1: Both temperatures within charging range
    battery_state_t normal_state = {};
    normal_state.ground_temperature_c = (MAX_CHARGING_TEMP_C + MIN_CHARGING_TEMP_C) / 2;
    normal_state.series_temperature_c = (MAX_CHARGING_TEMP_C + MIN_CHARGING_TEMP_C) / 2;
    battery_set_state(normal_state);
    TEST_ASSERT_TRUE(battery_in_charge_temp_range());

    // Test case 2: Ground temp too low
    battery_state_t low_ground_state = {};
    low_ground_state.ground_temperature_c = MIN_CHARGING_TEMP_C - 1.0f;
    low_ground_state.series_temperature_c = (MAX_CHARGING_TEMP_C + MIN_CHARGING_TEMP_C) / 2;
    battery_set_state(low_ground_state);
    TEST_ASSERT_FALSE(battery_in_charge_temp_range());

    // Test case 3: Ground temp too high
    battery_state_t high_ground_state = {};
    high_ground_state.ground_temperature_c = MAX_CHARGING_TEMP_C + 1.0f;
    high_ground_state.series_temperature_c = (MAX_CHARGING_TEMP_C + MIN_CHARGING_TEMP_C) / 2;
    battery_set_state(high_ground_state);
    TEST_ASSERT_FALSE(battery_in_charge_temp_range());

    // Test case 4: Series temp too low
    battery_state_t low_series_state = {};
    low_series_state.ground_temperature_c = (MAX_CHARGING_TEMP_C + MIN_CHARGING_TEMP_C) / 2;
    low_series_state.series_temperature_c = MIN_CHARGING_TEMP_C - 1.0f;
    battery_set_state(low_series_state);
    TEST_ASSERT_FALSE(battery_in_charge_temp_range());

    // Test case 5: Series temp too high
    battery_state_t high_series_state = {};
    high_series_state.ground_temperature_c = (MAX_CHARGING_TEMP_C + MIN_CHARGING_TEMP_C) / 2;
    high_series_state.series_temperature_c = MAX_CHARGING_TEMP_C + 1.0f;
    battery_set_state(high_series_state);
    TEST_ASSERT_FALSE(battery_in_charge_temp_range());

    // Test case 6: Edge cases - exactly at boundaries
    battery_state_t edge_case_state = {};
    edge_case_state.ground_temperature_c = MIN_CHARGING_TEMP_C + 0.001f;
    edge_case_state.series_temperature_c = MAX_CHARGING_TEMP_C - 0.001f;
    battery_set_state(edge_case_state);
    TEST_ASSERT_TRUE(battery_in_charge_temp_range());
}

void test_battery_in_discharge_temp_range(void)
{
    // Both temperatures within discharging range
    battery_state_t normal_state = {};
    normal_state.ground_temperature_c = (MAX_DISCHARGING_TEMP_C + MIN_DISCHARGING_TEMP_C) / 2;
    normal_state.series_temperature_c = (MAX_DISCHARGING_TEMP_C + MIN_DISCHARGING_TEMP_C) / 2;
    battery_set_state(normal_state);
    TEST_ASSERT_TRUE(battery_in_discharge_temp_range());

    // Ground temp too low
    battery_state_t low_ground_state = {};
    low_ground_state.ground_temperature_c = MIN_DISCHARGING_TEMP_C - 1.0f;
    low_ground_state.series_temperature_c = (MAX_DISCHARGING_TEMP_C + MIN_DISCHARGING_TEMP_C) / 2;
    battery_set_state(low_ground_state);
    TEST_ASSERT_FALSE(battery_in_discharge_temp_range());

    // Ground temp too high
    battery_state_t high_ground_state = {};
    high_ground_state.ground_temperature_c = MAX_DISCHARGING_TEMP_C + 1.0f;
    high_ground_state.series_temperature_c = (MAX_DISCHARGING_TEMP_C + MIN_DISCHARGING_TEMP_C) / 2;
    battery_set_state(high_ground_state);
    TEST_ASSERT_FALSE(battery_in_discharge_temp_range());

    // Series temp too low
    battery_state_t low_series_state = {};
    low_series_state.ground_temperature_c = (MAX_DISCHARGING_TEMP_C + MIN_DISCHARGING_TEMP_C) / 2;
    low_series_state.series_temperature_c = MIN_DISCHARGING_TEMP_C - 1.0f;
    battery_set_state(low_series_state);
    TEST_ASSERT_FALSE(battery_in_discharge_temp_range());

    // Series temp too high
    battery_state_t high_series_state = {};
    high_series_state.ground_temperature_c = (MAX_DISCHARGING_TEMP_C + MIN_DISCHARGING_TEMP_C) / 2;
    high_series_state.series_temperature_c = MAX_DISCHARGING_TEMP_C + 1.0f;
    battery_set_state(high_series_state);
    TEST_ASSERT_FALSE(battery_in_discharge_temp_range());

    // Edge cases - exactly at boundaries
    battery_state_t edge_case_state = {};
    edge_case_state.ground_temperature_c = MIN_DISCHARGING_TEMP_C + 0.001f;
    edge_case_state.series_temperature_c = MAX_DISCHARGING_TEMP_C - 0.001f;
    battery_set_state(edge_case_state);
    TEST_ASSERT_TRUE(battery_in_discharge_temp_range());
}

void test_battery_balancing_needed(void)
{
    // Voltage difference below threshold - no balancing needed
    battery_state_t balanced_state = {};
    balanced_state.upper_cell_voltage_v = 3.7f;
    balanced_state.lower_cell_voltage_v = 3.69f; // Difference = 0.01V
    battery_set_state(balanced_state);
    TEST_ASSERT_FALSE(battery_balancing_needed());

    // Voltage difference exactly at threshold - no balancing needed
    battery_state_t at_threshold_state = {};
    at_threshold_state.upper_cell_voltage_v = 3.7f + BALANCE_THRESHOLD_V;
    at_threshold_state.lower_cell_voltage_v = 3.7f;
    battery_set_state(at_threshold_state);
    TEST_ASSERT_FALSE(battery_balancing_needed()); // Should return false when exactly at threshold

    // Voltage difference just above threshold - balancing needed
    battery_state_t slightly_unbalanced_state = {};
    slightly_unbalanced_state.upper_cell_voltage_v = 3.7f + BALANCE_THRESHOLD_V + 0.001f;
    slightly_unbalanced_state.lower_cell_voltage_v = 3.7f;
    battery_set_state(slightly_unbalanced_state);
    TEST_ASSERT_TRUE(battery_balancing_needed());

    // Significant voltage difference - balancing needed
    battery_state_t very_unbalanced_state = {};
    very_unbalanced_state.upper_cell_voltage_v = 3.8f;
    very_unbalanced_state.lower_cell_voltage_v = 3.6f; // Difference = 0.2V
    battery_set_state(very_unbalanced_state);
    TEST_ASSERT_TRUE(battery_balancing_needed());

    // Lower cell higher voltage - balancing needed
    battery_state_t reverse_unbalanced_state = {};
    reverse_unbalanced_state.upper_cell_voltage_v = 3.6f;
    reverse_unbalanced_state.lower_cell_voltage_v = 3.8f; // Difference = 0.2V
    battery_set_state(reverse_unbalanced_state);
    TEST_ASSERT_TRUE(battery_balancing_needed());

    // Equal voltages - no balancing needed
    battery_state_t perfectly_balanced_state = {};
    perfectly_balanced_state.upper_cell_voltage_v = 3.7f;
    perfectly_balanced_state.lower_cell_voltage_v = 3.7f; // Difference = 0.0V
    battery_set_state(perfectly_balanced_state);
    TEST_ASSERT_FALSE(battery_balancing_needed());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_set_upper_discharge);
    RUN_TEST(test_set_lower_discharge);
    RUN_TEST(test_battery_is_fully_charged);
    RUN_TEST(test_battery_is_depleted);
    RUN_TEST(test_battery_in_charge_temp_range);
    RUN_TEST(test_battery_in_discharge_temp_range);
    RUN_TEST(test_battery_balancing_needed);
    return UNITY_END();
}