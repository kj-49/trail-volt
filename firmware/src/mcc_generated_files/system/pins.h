/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  1.1.0
*/

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H_INCLUDED
#define PINS_H_INCLUDED

#include <avr/io.h>
#include "./port.h"

//get/set IO_PD0 aliases
#define IO_PD0_SetHigh() do { PORTD_OUTSET = 0x1; } while(0)
#define IO_PD0_SetLow() do { PORTD_OUTCLR = 0x1; } while(0)
#define IO_PD0_Toggle() do { PORTD_OUTTGL = 0x1; } while(0)
#define IO_PD0_GetValue() (VPORTD.IN & (0x1 << 0))
#define IO_PD0_SetDigitalInput() do { PORTD_DIRCLR = 0x1; } while(0)
#define IO_PD0_SetDigitalOutput() do { PORTD_DIRSET = 0x1; } while(0)
#define IO_PD0_SetPullUp() do { PORTD_PIN0CTRL  |= PORT_PULLUPEN_bm; } while(0)
#define IO_PD0_ResetPullUp() do { PORTD_PIN0CTRL  &= ~PORT_PULLUPEN_bm; } while(0)
#define IO_PD0_SetInverted() do { PORTD_PIN0CTRL  |= PORT_INVEN_bm; } while(0)
#define IO_PD0_ResetInverted() do { PORTD_PIN0CTRL  &= ~PORT_INVEN_bm; } while(0)
#define IO_PD0_DisableInterruptOnChange() do { PORTD.PIN0CTRL = (PORTD.PIN0CTRL & ~PORT_ISC_gm) | 0x0 ; } while(0)
#define IO_PD0_EnableInterruptForBothEdges() do { PORTD.PIN0CTRL = (PORTD.PIN0CTRL & ~PORT_ISC_gm) | 0x1 ; } while(0)
#define IO_PD0_EnableInterruptForRisingEdge() do { PORTD.PIN0CTRL = (PORTD.PIN0CTRL & ~PORT_ISC_gm) | 0x2 ; } while(0)
#define IO_PD0_EnableInterruptForFallingEdge() do { PORTD.PIN0CTRL = (PORTD.PIN0CTRL & ~PORT_ISC_gm) | 0x3 ; } while(0)
#define IO_PD0_DisableDigitalInputBuffer() do { PORTD.PIN0CTRL = (PORTD.PIN0CTRL & ~PORT_ISC_gm) | 0x4 ; } while(0)
#define IO_PD0_EnableInterruptForLowLevelSensing() do { PORTD.PIN0CTRL = (PORTD.PIN0CTRL & ~PORT_ISC_gm) | 0x5 ; } while(0)
#define PD0_SetInterruptHandler IO_PD0_SetInterruptHandler

/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize();

/**
 * @ingroup  pinsdriver
 * @brief Default Interrupt Handler for IO_PD0 pin. 
 *        This is a predefined interrupt handler to be used together with the IO_PD0_SetInterruptHandler() method.
 *        This handler is called every time the IO_PD0 ISR is executed. 
 * @pre PIN_MANAGER_Initialize() has been called at least once
 * @param none
 * @return none
 */
void IO_PD0_DefaultInterruptHandler(void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt Handler Setter for IO_PD0 pin input-sense-config functionality.
 *        Allows selecting an interrupt handler for IO_PD0 at application runtime
 * @pre PIN_MANAGER_Initialize() has been called at least once
 * @param InterruptHandler function pointer.
 * @return none
 */
void IO_PD0_SetInterruptHandler(void (* interruptHandler)(void)) ; 
#endif /* PINS_H_INCLUDED */
