#include "menu.h"
#include <stdint.h>

static menu_state_t menu_state = {MODE_DUAL};

static const uint8_t num_modes = 3;
static mode_e options[num_modes] = {MODE_DUAL, MODE_RECEIVING, MODE_SUPPLYING};
static uint8_t mode_index = 0; 

void menu_update_state(encoder_event_e encoder_event) {
    /*
     * We need to map rotary encoder events to the menu state.
     * - Clockwise rotations => downward navigation of the menu.
     * - Counter-clockwise rotations => upward navigation of the menu.
     */
    switch (encoder_event) {
        case ENCODER_EVENT_CLOCKWISE:
            mode_index = (mode_index + 1) % num_modes;
            break;
        case ENCODER_EVENT_COUNTERCLOCKWISE:
            mode_index = (mode_index - 1 + num_modes) % num_modes; // Handles negative indices properly
            break;
        default:
            // Other events are not important here.
            break;
    }

    menu_state.selected_mode = options[mode_index];
}

mode_e menu_get_selected_state() {
  return menu_state.selected_mode;
}