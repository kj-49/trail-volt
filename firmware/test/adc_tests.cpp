#include "unity/unity.h"
#include "fff.h"
#include "../src/controller/adc.h"
#include "../src/controller/hal.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(int, hal_analog_read, int);
FAKE_VOID_FUNC(hal_delay, unsigned long);

void setUp(void)
{
    RESET_FAKE(hal_analog_read);
    RESET_FAKE(hal_delay);
}

void tearDown(void)
{
}

void test_adc_read(void)
{
    hal_analog_read_fake.return_val = 512;

    float voltage = adc_read(1, 2.0f);

    float expected_voltage = ((512.0f / 1023.0f) * ADC_VREF_CALIBRATED * 2.0f);

    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_voltage, voltage);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_adc_read);
    return UNITY_END();
}
