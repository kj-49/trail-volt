/* 
 * File:   adc.c
 * Author: Kyle James
 *
 * Created on February 16, 2025, 9:22 AM
 */

#include "adc.h"
#include <avr/io.h>

void ADC0_configure() {
    // Disable ADC0 module
    ADC0.CTRLA &= ~(ADC_ENABLE_bm);
    // Disable ADC0 interrupts
    ADC0.INTCTRL &= ~(ADC_RESRDY_bm | ADC_WCMP_bm);
    // Clear ADC0 interrupt flags
    ADC0.INTFLAGS = (ADC_RESRDY_bm | ADC_WCMP_bm);
    
    // Configure ADC0 module registers
    ADC0.CTRLB = (ADC_SAMPNUM_ACC4_gc);             /* SAMPNUM (4 results accumulated) */
    ADC0.CTRLC = (ADC_PRESC_DIV2_gc);               /* PRESC (CLK_PER divided by 2) */
    ADC0.CTRLD = (ADC_INITDLY_DLY0_gc)              /* INITDLY (DLY0) */
				|(ADC_SAMPDLY_DLY0_gc);             /* SAMPDLY (DLY0) */
    ADC0.CTRLE = (ADC_WINCM_NONE_gc);               /* WINCM (No Window Comparison) */
    ADC0.SAMPCTRL = 0x0;                               /* SAMPLEN (0x0) */
    ADC0.MUXPOS = (ADC_MUXPOS_AIN0_gc);              /* MUXPOS (AIN0) */
    ADC0.MUXNEG = (ADC_MUXNEG_GND_gc);               /* MUXNEG (GND) */
    ADC0.COMMAND = (0 << ADC_STCONV_bp)               /* STCONV (disabled) */
				|(0 << ADC_SPCONV_bp);              /* SPCONV (disabled) */
    ADC0.EVCTRL = (0 << ADC_STARTEI_bp);             /* STARTEI (disabled) */
    ADC0.DBGCTRL = (0 << ADC_DBGRUN_bp);              /* DBGRUN (disabled) */
    ADC0.WINHT = 0x0;                               /* WINHT (0x0) */
    ADC0.WINLT = 0x0;                               /* WINLT (0x0) */
    ADC0.INTCTRL = (0 << ADC_RESRDY_bp)               /* RESRDY (disabled) */
				|(0 << ADC_WCMP_bp);                /* WCMP (disabled) */

    ADC0.CTRLA = (1 << ADC_ENABLE_bp)               /* ENABLE (enabled) */
				|(0 << ADC_FREERUN_bp)              /* FREERUN (disabled) */
				|(ADC_RESSEL_12BIT_gc)              /* RESSEL (12-bit mode) */
				|(0 << ADC_RUNSTBY_bp)              /* RUNSTBY (disabled) */
				|(ADC_CONVMODE_SINGLEENDED_gc)      /* CONVMODE (SINGLEENDED) */
				|(0 << ADC_LEFTADJ_bp);             /* LEFTADJ (disabled) */
}
void ADC0_select_channel(adc_channel_t channel) {
    ADC0.MUXPOS = channel;
}
void ADC0_begin_conversion() {
    ADC0.COMMAND |= ADC_STCONV_bm;
}
int ADC0_conversion_is_done() {
    return !(ADC0.COMMAND & ADC_STCONV_bm);
}
uint16_t ADC0_get_conversion_result() {
    return (uint16_t)ADC0.RES;
}

void ADC0_clear_conversion_isr_flag() {
    ADC0.INTFLAGS = ADC_RESRDY_bm;
}

