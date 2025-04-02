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

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_set_upper_discharge);
    RUN_TEST(test_set_lower_discharge);
    return UNITY_END();
}