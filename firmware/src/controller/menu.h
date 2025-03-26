#ifndef MENU_H
#define	MENU_H

#include "encoder.h"
#include "mode.h"

typedef struct {
    mode_e selected_mode;
} menu_state_t;

/**
 * @brief  Updates the selected item.
 * @param  encoder_event: The event to update the state based on.
 */
void menu_update_state(encoder_event_e encoder_event);

/**
 * @brief  Retrieves current select state.
 */
mode_e menu_get_selected_state();

#endif /* MENU_H */

