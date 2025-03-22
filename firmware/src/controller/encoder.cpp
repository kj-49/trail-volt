#include "encoder.h"
#include "gpio.h"

static encoder_state_t encoder_state;

/**
 * @brief  Updates the encoder state based on the current encoder inputs.
 */
static void update_state() {
    int sw = digitalRead(ENCODER_SW_PIN);
    int clk = digitalRead(ENCODER_CLK_PIN);
    int dt = digitalRead(ENCODER_DT_PIN);
    
    encoder_state.sw = sw;
    encoder_state.clk = clk;
    encoder_state.dt = dt;
}

void encoder_init() {
    update_state();
}

encoder_event_e encoder_get_event() {

    encoder_state_t previous_state = encoder_state;
    update_state();
    encoder_state_t current_state = encoder_state;

    // Prioritize button presses
    if (current_state.sw == LOW && previous_state.sw == HIGH) {
      /*
       * We will detect button presses on the first time we detect a LOW after HIGH.
       * With this method, we should ensure our encoder state is only updated by calling this fuction.
       * If our state is updated independent of this function, we could miss a transition from high to low.
       * or button presses could be missed.
       */
      return ENCODER_EVENT_BUTTON_PRESS;
    }

    // If a pulse has occured on the clk pin
    if (current_state.clk != previous_state.clk) {
        // If the clk is different from dt, the encoder is moving clockwise
        if (current_state.clk != current_state.dt) {
          return ENCODER_EVENT_CLOCKWISE;
        } else {
          return ENCODER_EVENT_COUNTERCLOCKWISE;
        }
    }

    return ENCODER_EVENT_NONE;
}