#include "supplying.h"
#include "hal.h"
#include "gpio.h"

void supplying_disable()
{
    hal_digital_write(USB_ENABLE_PIN, LOW);
}

void supplying_enable()
{
    hal_digital_write(USB_ENABLE_PIN, HIGH);
}
