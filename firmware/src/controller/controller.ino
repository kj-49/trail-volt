
#include "display.h"
#include "mode.h"
#include "charging.h"
#include "gpio.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "battery.h"
#include "state_manager.h"

#define LOOP_DELAY_MS 10

void setup() {
    Serial.begin(9600);
    Serial.println("Starting.");

    charging_init();
    display_init();

    display_draw_logo();
    delay(2500);

    gpio_init();
    state_manager_init();
}

void loop() {

    state_manager_update_sensors();
    
    state_manager_update_application_mode();

    state_manager_apply_hardware_updates();
    
    delay(10);
}
