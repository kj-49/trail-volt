#include "unity/unity.h"
#include "fff.h"
#include "../src/controller/temperature.h"
#include "../src/controller/adc.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(float, adc_read, int, float);

void setUp(void) 
{
    RESET_FAKE(adc_read);
}

void tearDown(void) 
{
}

void test_get_resistance(void)
{
    adc_read_fake.return_val = 2.0f;
    
    float resistance = get_resistance(1); 

    float expected_resistance = (2.0f * SERIES_RESISTOR) / (ADC_VREF_CALIBRATED - 2.0f);

    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_resistance, resistance);
}

void test_get_resistance_should_be_zero_if_ground(void)
{
    adc_read_fake.return_val = 0;
    
    float resistance = get_resistance(1); 

    // Since low-side, an ADC reading of max voltage should translate to a open
    TEST_ASSERT_FLOAT_WITHIN(0.025f, 0, resistance);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_resistance);
    RUN_TEST(test_get_resistance_should_be_zero_if_ground);
    return UNITY_END();
}
