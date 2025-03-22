#include "encoder.h"
#include "gpio.h"

static encoder_event_e pending_event;

void encoder_init() {
    pending_event = ENCODER_EVENT_NONE;
}

encoder_event_e encoder_get_event() {
    encoder_event_e event = pending_event;
    // Mark as read
    pending_event = ENCODER_EVENT_NONE;
    return event;
}

void encoder_handle_clk_rising() {
    int dt = digitalRead(ENCODER_DT_PIN);

    // If the clk is different from dt, the encoder is moving clockwise
    if (dt != HIGH) {
      pending_event = ENCODER_EVENT_CLOCKWISE;
    } else {
      pending_event = ENCODER_EVENT_COUNTERCLOCKWISE;
    }
}

void encoder_handle_btn_press() {
    pending_event = ENCODER_EVENT_BUTTON_PRESS;
}