#ifndef ENCODER_H
#define	ENCODER_H

typedef enum {
  ENCODER_EVENT_BUTTON_CONFIRM_PRESS = 0,
  ENCODER_EVENT_BUTTON_CHANGE_PRESS = 1,
  ENCODER_EVENT_NONE = 2,
} encoder_event_e;

/**
 * @brief  Initializes encoder state.
 */
void encoder_init();

/**
 * @brief  Gets the last encoder event.
 * @return The encoder event type.
 */
encoder_event_e encoder_get_event();

/**
 * @brief  Handles the btn 2 rising interrupt.
 */
void encoder_handle_btn_2_press();
/**
 * @brief  Handles the btn 1 rising interrupt.
 */
void encoder_handle_btn_1_press();

#endif /* ENCODER_H */

