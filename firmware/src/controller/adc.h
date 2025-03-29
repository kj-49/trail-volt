
#ifndef ADC_H
#define	ADC_H

const float ADC_VREF_CALIBRATED = 4.916;

/**
 * @brief  Reads value from specified digital pin using ADC
 * @param  pin: The digital pin number to read from
 * @param  voltage_divider_ratio: The ratio by which the voltage is divided
 * @return ADC reading in volts
 */
float adc_read(int pin, float voltage_divider_ratio);

#endif	/* ADC_H */
