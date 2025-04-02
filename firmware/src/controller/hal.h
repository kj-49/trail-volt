#ifndef HAL_H
#define HAL_H

#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

int hal_digital_read(int pin);

void hal_digital_write(int pin, int value);

void hal_pin_mode(int pin, int value);

int hal_analog_read(int pin);

void hal_analog_write(int pin, int value);

void hal_delay(unsigned long milliseconds);

unsigned long hal_millis();

#endif /* HAL_H */
