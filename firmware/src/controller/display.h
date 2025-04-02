
#ifndef DISPLAY_H
#define	DISPLAY_H

#include "mode.h"
#include "battery.h"
#include "charging.h"

// Un-comment the definition below if using the SSD1306 instead of the SH1106.
// #define USE_SSD1306

#define DISPLAY_I2C_ADDRESS 0x3c

#define DISP_WIDTH 128
#define DISP_HEIGHT 64

#define OLED_RESET -1 

/**
 * @brief  Initializes the display.
 */
void display_init();

/**
 * @brief  Refreshes the main display.
 */
void display_update();

/**
 * @brief  Displays bootup logo.
 */
void display_draw_logo();

#endif	/* DISPLAY_H */

