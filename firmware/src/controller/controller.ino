// main.ino
#include "sensors.h"
#include "display.h"
#include "state.h"
#include "power.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

volatile application_state_t application_state = STATE_MONITORING;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

sensor_state_t sensor_state = {0, 0, 0, 0, false};

void setup() {
    
    configure_display(&display);
    draw_logo(&display);
    delay(2500);

    pinMode(CELL1_VOLTAGE_PIN, INPUT);
    pinMode(CELL2_VOLTAGE_PIN, INPUT);
    pinMode(CELL1_TEMP_PIN, INPUT);
    pinMode(CELL2_TEMP_PIN, INPUT);
    pinMode(CHARGE_RATE_PIN, INPUT);
}

void loop() {
    switch(application_state) {
        case STATE_MONITORING:
            check_update_state(&application_state);
            update_sensor_state(&sensor_state);
            update_display(&display, &sensor_state, &application_state);
            break;
        case STATE_CHARGING:
            while (is_charging()) {}
            application_state = STATE_MONITORING;
            break;
        case STATE_SLEEP:
            break;
        default:
            break;
    }
}
