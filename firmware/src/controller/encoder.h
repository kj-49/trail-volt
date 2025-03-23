#ifndef ENCODER_H
#define	ENCODER_H

typedef enum {
  ENCODER_EVENT_BUTTON_PRESS = 0,
  ENCODER_EVENT_CLOCKWISE = 1,
  ENCODER_EVENT_COUNTERCLOCKWISE = 2,
  ENCODER_EVENT_NONE = 3,
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
 * @brief  Handles the clk rising interrupt.
 */
void encoder_handle_clk_rising();
/**
 * @brief  Handles the button press interrupt.
 */
void encoder_handle_btn_press();

#endif /* ENCODER_H */

