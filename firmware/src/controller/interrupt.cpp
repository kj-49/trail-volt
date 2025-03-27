#include "interrupt.h"
#include "encoder.h"
#include <Arduino.h>
#include "gpio.h"

void interrupt_attach_encoder()
{
    attachInterrupt(digitalPinToInterrupt(BTN_2_PIN), encoder_handle_btn_2_press, RISING);
    attachInterrupt(digitalPinToInterrupt(BTN_1_PIN), encoder_handle_btn_1_press, RISING);
}