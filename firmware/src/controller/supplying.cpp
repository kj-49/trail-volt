#include "supplying.h"
#include <Arduino.h>
#include "gpio.h"

bool supplying_disable() {
    digitalWrite(USB_ENABLE_PIN, LOW);
}

bool supplying_enable() {
    digitalWrite(USB_ENABLE_PIN, HIGH);
}
