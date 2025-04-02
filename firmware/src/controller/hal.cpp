#include "hal.h"
#include <Arduino.h>

int hal_digital_read(int pin) {
    return digitalRead(pin);
}

void hal_digital_write(int pin, int value) {
    digitalWrite(pin, value);
}

void hal_pin_mode(int pin, int value) {
    pinMode(pin, value);
}


int hal_analog_read(int pin) {
    return analogRead(pin);
}

void hal_analog_write(int pin, int value) {
    analogWrite(pin, value);
}

void hal_delay(unsigned long milliseconds) {
    delay(milliseconds);
}

unsigned long hal_millis() {
    return millis();
}
