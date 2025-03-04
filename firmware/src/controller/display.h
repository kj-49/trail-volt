
#ifndef DISPLAY_H
#define	DISPLAY_H

#include "mode.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "battery.h"
#include "charging.h"



#define i2c_address 0x3c

#define DISP_WIDTH 128
#define DISP_HEIGHT 64

#define OLED_RESET -1 

/**
 * @brief  Initializes the display.
 */
void display_init();

/**
 * @brief  Refreshes the main display.
 * @param  battery_state: A pointer to the battery state.
 * @param  charging_state: A pointer to the battery state.
 * @param  mode_e: The current mode.
 */
void display_update(const battery_state_t *battery_state, const charging_state_t *charging_state, mode_e mode);

/**
 * @brief  Displays bootup logo.
 */
void display_draw_logo();

#endif	/* DISPLAY_H */

