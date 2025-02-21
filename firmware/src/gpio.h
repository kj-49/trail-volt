/* 
 * File:   gpio.h
 * Author: Kyle James
 *
 * Created on February 21, 2025, 12:12 PM
 */

#ifndef GPIO_H
#define	GPIO_H

typedef struct gpio {
    PORT_t *port;
    uint8_t pin;
} gpio_t;

/**
 * @brief  Sets pin as output.
 * @param  gpio: The pin to set.
 */
inline void set_as_output(gpio_t gpio)
{
    gpio.port->DIRSET = (1 << gpio.pin);
}

/**
 * @brief  Sets pin as input.
 * @param  gpio: The pin to set.
 */
inline void set_as_input(gpio_t gpio)
{
    gpio.port->DIRCLR = (1 << gpio.pin);
}

/**
 * @brief  Sets pin as input.
 * @param  gpio: The pin to set.
 */
inline bool read_pin(gpio_t gpio)
{
    return (gpio.port->IN & (1 << gpio.pin));
}

/**
 * @brief  Sets pin to high.
 * @param  gpio: The pin to set.
 */
inline void set_pin_high(gpio_t gpio)
{
    gpio.port->OUTSET = (1 << gpio.pin);
}

/**
 * @brief  Sets pin to low.
 * @param  gpio: The pin to set.
 */
inline void set_pin_low(gpio_t gpio)
{
    gpio.port->OUTCLR = (1 << gpio.pin);
}

#endif	/* GPIO_H */

