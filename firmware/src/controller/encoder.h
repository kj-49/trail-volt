#ifndef ENCODER_H
#define	ENCODER_H

typedef struct {
  bool clk;
  bool dt;
  bool sw;
} encoder_state_t;

typedef enum {
  ENCODER_EVENT_BUTTON_PRESS,
  ENCODER_EVENT_CLOCKWISE,
  ENCODER_EVENT_COUNTERCLOCKWISE,
  ENCODER_EVENT_NONE,
} encoder_event_e;

/**
 * @brief  Initializes encoder state.
 */
void encoder_init();

/**
 * @brief  Determines if any encoder events have occurs based on the
 *         current and previous encoder values.
 * @return The encoder event type.
 */
encoder_event_e encoder_get_event();


#endif /* ENCODER_H */

