/* 
 * File:   pwm.h
 * Author: Kyle James
 *
 * Created on February 21, 2025, 12:36 PM
 */

#ifndef PWM_H
#define	PWM_H

#define PWM_FREQ 2000


/**
 * @brief  Configures PWM wave on TCB2 but does not enable.
 */
void TCB2_init_pwm();


/**
 * @brief  Sets the duty cycle of the PWM wave on TCB2.
 * @param perc_duty_cycle: An integer value [0-100] representing the percentage duty cycle of the wave.
 */
void TCB2_set_duty_cycle(int perc_duty_cycle);


/**
 * @brief  Enables the PWM wave for TCB2.
 */
void TCB2_enable();

/**
 * @brief  Disables the PWM wave for TCB2.
 */
void TCB2_disable();

#endif	/* PWM_H */

