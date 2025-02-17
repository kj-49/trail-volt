/* 
 * File:   display.h
 * Author: Kyle James
 *
 * Created on February 15, 2025, 6:40 PM
 */

#ifndef DISPLAY_H
#define	DISPLAY_H

#include "sensors.h"
#include "../lib/u8g2.X/u8g2.h"
#include "../lib/twi.X/twi-host.X/peripherals/TWI/TWI_host.h"
#include "state.h"

void configure_display(u8g2_t *u8g2);
void update_display(u8g2_t *u8g2, const sensor_state_t *sensor_state, volatile const application_state_t *application_state);
void draw_logo(u8g2_t *u8g2);

#endif	/* DISPLAY_H */

