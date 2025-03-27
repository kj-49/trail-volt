#include "encoder.h"
#include "gpio.h"

static encoder_event_e pending_event;
static unsigned long last_button_1_press_ms = 0;
static unsigned long last_button_2_press_ms = 0;

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

void encoder_handle_btn_1_press()
{
    unsigned long current_count = millis();

    /*
     * To debounce button press, only register if last button press
     * was longer than 50ms ago.
     */
    if (current_count - last_button_1_press_ms > 250) {
        pending_event = ENCODER_EVENT_BUTTON_CONFIRM_PRESS;
    }

    last_button_1_press_ms = current_count;   
}

void encoder_handle_btn_2_press()
{
    unsigned long current_count = millis();

    /*
     * To debounce button press, only register if last button press
     * was longer than 50ms ago.
     */
    if (current_count - last_button_2_press_ms > 250) {
        pending_event = ENCODER_EVENT_BUTTON_CHANGE_PRESS;
    }

    last_button_2_press_ms = current_count;   
}