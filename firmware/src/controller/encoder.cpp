#include "encoder.h"
#include "gpio.h"
#include "hal.h"

static encoder_event_e pending_event;
static unsigned long last_button_press_ms = 0;

void encoder_init()
{
    pending_event = ENCODER_EVENT_NONE;
}

encoder_event_e encoder_get_event()
{
    encoder_event_e event = pending_event;
    // Mark as read
    pending_event = ENCODER_EVENT_NONE;
    return event;
}

void encoder_handle_clk_rising()
{
    int dt = hal_digital_read(ENCODER_DT_PIN);

    // If the clk is different from dt, the encoder is moving clockwise
    if (dt != true) {
      pending_event = ENCODER_EVENT_CLOCKWISE;
    } else {
      pending_event = ENCODER_EVENT_COUNTERCLOCKWISE;
    }
}

void encoder_handle_btn_press()
{
    unsigned long current_count = hal_millis();

    /*
     * To debounce button press, only register if last button press
     * was longer than 50ms ago.
     */
    if (current_count - last_button_press_ms > 250) {
        pending_event = ENCODER_EVENT_BUTTON_PRESS;
    }

    last_button_press_ms = current_count;   
}