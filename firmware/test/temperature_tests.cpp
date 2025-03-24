#include "unity/unity.h"
#include "../src/controller/temperature.h"

// Mocked adc_read function
float adc_read(int pin, float division) 
{
    if (pin == 1) {
        return 2.0f;
    } else if (pin == 2) {
        return 3.0f;
    }
    return 0.0f;
}

void setUp(void) 
{
}

void tearDown(void) 
{
}

void test_get_resistance(void)
{
    // Test case 1: Pin 1, voltage of 2V
    float resistance = get_resistance(1);

    TEST_ASSERT_FLOAT_WITHIN(0.1f, 16666.67f, resistance);
    
    // Test case 2: Pin 2, voltage of 3V
    resistance = get_resistance(2);

    TEST_ASSERT_FLOAT_WITHIN(0.1f, 25000.0f, resistance);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_get_resistance);
    return UNITY_END();
}