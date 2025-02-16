/* 
 * File:   adc.h
 * Author: Kyle James
 *
 * Created on February 16, 2025, 9:22 AM
 */

#ifndef ADC_H
#define	ADC_H

#include <stdint.h>

typedef enum
{
    ADC0_CHANNEL_AIN0         = 0x0,   /**< PD0*/  
    ADC0_CHANNEL_AIN1         = 0x1,   /**< PD1*/  
    ADC0_CHANNEL_AIN2         = 0x2,   /**< PD2*/  
    ADC0_CHANNEL_AIN3         = 0x3,   /**< PD3*/  
    ADC0_CHANNEL_AIN4         = 0x4,   /**< PD4*/  
    ADC0_CHANNEL_AIN5         = 0x5,   /**< PD5*/  
    ADC0_CHANNEL_AIN6         = 0x6,   /**< PD6*/  
    ADC0_CHANNEL_AIN7         = 0x7,   /**< PD7*/  
    ADC0_CHANNEL_AIN8         = 0x8,   /**< PE0*/  
    ADC0_CHANNEL_AIN9         = 0x9,   /**< PE1*/  
    ADC0_CHANNEL_AIN10        = 0xa,   /**< PE2*/  
    ADC0_CHANNEL_AIN11        = 0xb,   /**< PE3*/  
    ADC0_CHANNEL_AIN16        = 0x10,  /**< PF0*/  
    ADC0_CHANNEL_AIN17        = 0x11,  /**< PF1*/  
    ADC0_CHANNEL_AIN18        = 0x12,  /**< PF2*/  
    ADC0_CHANNEL_AIN19        = 0x13,  /**< PF3*/  
    ADC0_CHANNEL_AIN20        = 0x14,  /**< PF4*/  
    ADC0_CHANNEL_AIN21        = 0x15,  /**< PF5*/  
    ADC0_CHANNEL_GND          = 0x40,  /**< Ground*/  
    ADC0_CHANNEL_TEMPSENSE    = 0x42,  /**< Temperature sensor*/  
    ADC0_CHANNEL_VDDDIV10     = 0x44,  /**< VDD/10*/  
    ADC0_CHANNEL_VDDIO2DIV10  = 0x45,  /**< VDDIO2/10*/  
    ADC0_CHANNEL_DAC0         = 0x48,  /**< DAC0*/  
    ADC0_CHANNEL_DACREF0      = 0x49,  /**< AC0 DAC Voltage Reference*/  
    ADC0_CHANNEL_DACREF1      = 0x4a,  /**< AC1 DAC Voltage Reference*/  
    ADC0_CHANNEL_DACREF2      = 0x4b   /**< AC2 DAC Voltage Reference*/  
} adc_channel_t;

void ADC0_configure();
void ADC0_select_channel(adc_channel_t channel);
void ADC0_begin_conversion();
int ADC0_conversion_is_done();
uint16_t ADC0_get_conversion_result();
void ADC0_clear_conversion_isr_flag();

#endif	/* ADC_H */

