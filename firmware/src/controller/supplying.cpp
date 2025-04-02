#include "supplying.h"
#include "hal.h"
#include "gpio.h"

bool supplying_disable()
{
    hal_digital_write(USB_ENABLE_PIN, LOW);
}

bool supplying_enable()
{
    hal_digital_write(USB_ENABLE_PIN, HIGH);
}
