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

    float expected_resistance = ((ADC_VREF_CALIBRATED * SERIES_RESISTOR) / 2.0f) - SERIES_RESISTOR;
    
    TEST_ASSERT_FLOAT_WITHIN(0.1f, expected_resistance, resistance);
}

void test_get_resistance_should_be_zero_if_vcc(void)
{
    adc_read_fake.return_val = ADC_VREF_CALIBRATED;
    
    float resistance = get_resistance(1); 

    // Since high-side, an ADC reading of max voltage should translate to a short
    TEST_ASSERT_FLOAT_WITHIN(0.025f, 0, resistance);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_resistance);
    RUN_TEST(test_get_resistance_should_be_zero_if_vcc);
    return UNITY_END();
}
