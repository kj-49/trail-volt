#include "gpio.h"
#include "hal.h"

void gpio_init()
{
    hal_pin_mode(THERMISTOR_1_PIN, INPUT);
    hal_pin_mode(THERMISTOR_2_PIN, INPUT);

    hal_pin_mode(TOTAL_CELL_ADC_PIN, INPUT);
    hal_pin_mode(LOWER_CELL_ADC_PIN, INPUT);

    hal_pin_mode(ENCODER_DT_PIN, INPUT);
    hal_pin_mode(ENCODER_SW_PIN, INPUT_PULLUP);
    hal_pin_mode(ENCODER_CLK_PIN, INPUT);

    // Keep pin high by default (shutdown off)
    hal_pin_mode(H_BRIDGE_SHUT_DOWN_PIN_AL, OUTPUT);
    hal_digital_write(H_BRIDGE_SHUT_DOWN_PIN_AL, HIGH);

    // Initialize to low
    hal_pin_mode(USB_ENABLE_PIN, OUTPUT);
    hal_pin_mode(USB_ENABLE_PIN, LOW);

    hal_pin_mode(C1_BALANCING_PIN, OUTPUT);
    hal_pin_mode(C2_BALANCING_PIN, OUTPUT);

    hal_pin_mode(CHARGE_PWM_PIN, OUTPUT);

}
