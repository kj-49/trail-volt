#include "menu.h"

static menu_state_t menu_state = {MODE_SUPPLYING};

void menu_update_state(encoder_event_e encoder_event) {
    /*
     * We need to map rotary encoder events to the menu state.
     * - Clockwise rotations => downward navigation of the menu.
     * - Counter-clockwise rotations => upward navigation of the menu.
     */
    switch (encoder_event) {
        case ENCODER_EVENT_CLOCKWISE:
            menu_state.selected_mode = MODE_RECEIVING;
            break;
        case ENCODER_EVENT_COUNTERCLOCKWISE:
            menu_state.selected_mode = MODE_SUPPLYING;
            break;
        default:
            // Other events are not important here.
            break;
    }
}

mode_e menu_get_selected_state() {
  return menu_state.selected_mode;
}