#include "interrupt.h"
#include "encoder.h"
#include <Arduino.h>
#include "gpio.h"

void interrupt_attach_encoder() {
    attachInterrupt(digitalPinToInterrupt(ENCODER_CLK_PIN), encoder_handle_clk_rising, RISING);
    attachInterrupt(digitalPinToInterrupt(ENCODER_SW_PIN), encoder_handle_btn_press, FALLING);
}