/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Implementation of the single synch byte method.
 *
 *      This file contain the implementation of the single SYNCH byte
 *      synchronization method. The UART RXC interrupt service routine (ISR)
 *      is a part of this implementation. To implement a communication protocol,
 *      code to handle reception of UART data must be added in this file or
 *      double_synch_byte.c depending on which method is used.
 *
 * \par Application note:
 *      AVR054 Synchronization of the internal RC oscillator
 *
 * \par Documentation:
 *      For comprehensive code documentation, supported compilers, compiler
 *      settings and supported devices see readme.html
 *
 * \author
 *      Atmel Corporation: http://www.atmel.com \n
 *      Support email: avr@atmel.com \n
 *
 * $Name: RELEASE_1_2 $
 * $Revision: 1.3 $
 * $RCSfile: single_synch_byte.c,v $
 * $Date: 2006/02/17 11:47:59 $  \n
 ******************************************************************************/

#include "online_synch.h"
#include "device_specific.h"
#include <ioavr.h>
#include <inavr.h>

#if defined(SYNCH_METHOD_SINGLE_SYNCH_BYTE)

extern unsigned char breakDetected;
extern unsigned char calStep;
extern unsigned char synchState;

unsigned char defaultOSCCAL;

void Initialize_Synchronization(void)
{
    // Initialize UART.
    SYNCH_USART_STATCTRL_REG_B = (1 << SYNCH_RXEN) | (1 << SYNCH_RXCIE);
    SYNCH_UBRRH = (SYNCH_UBRR >> 8);
    SYNCH_UBRRL = (SYNCH_UBRR & 0x00ff);

    // Set INT0 pin as input, no internal pullup.
    DDR_INT0 &= ~(1 << PIN_NUMBER_INT0);
    PORT_INT0 &= ~(1 << PIN_NUMBER_INT0);

    // If 8 bit timer is used, it must be started here.
    #if ! defined(NINE_BIT_TIMER)
    SYNCH_TIMER_PRESCALER_REGISTER |= (1 << CS00);
    #endif

    // Read default OSCCAL value from EEPROM.
    while(EECR & (1 << EEPROM_WRITE_ENABLE))
    {
    }
    EEAR = DEFAULT_OSCCAL_ADDRESS;
    EECR |= (1 << EERE);
    defaultOSCCAL = EEDR;
}


#pragma vector=SYNCH_USART_RXC_vect
__interrupt void UART_RXC_ISR(void)
{
    unsigned char temp;
    if (SYNCH_USART_STATCTRL_REG_A & (1 << SYNCH_FE))  // Frame error has occured.
    {
        PREPARE_FOR_SYNCH();
    }
    else
    {
        // Process character received by UART receiver.
        // It is necessary that the UART data register is read at this point
        // to clear the receive buffer. If the value read is not used, the
        // compiler will optimize away the reading of UART data register. In
        // this case the synchronization will not work.
        temp = SYNCH_UDR;
        PORTB = temp;
    }
}

// Force no optimization for this ISR.
// If this is changed, the timing will not be correct.
#pragma optimize=z 2
#pragma vector=SYNCH_EXT_INT_vect
__interrupt void SYNCH_EXT_INT_ISR(void)
{
#if defined(NINE_BIT_TIMER)
    unsigned int cycleCount;
#else
    unsigned char cycleCount;
#endif

#if defined(NINE_BIT_TIMER)
    // Stop Timer/Counter0.
    SYNCH_TIMER_PRESCALER_REGISTER &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));

    // Read Timer/Counter0.
    cycleCount = TCNT0;
    cycleCount |= ((SYNCH_TIMER_INT_FLAG_REGISTER & (1 << TOV0)) << (8 - TOV0));

    // Reset Timer/Counter0.
    TCNT0 = 0;
    SYNCH_TIMER_INT_FLAG_REGISTER = (1 << TOV0);   // Clear overflow flag.

    // Start Timer/Counter0.
    SYNCH_TIMER_PRESCALER_REGISTER = (1 << CS00);   // Timer/Counter1 runs at fclk.
#else
    // Read Timer/Counter0.
    cycleCount = TCNT0;

    // Reset Timer/Counter0.
    TCNT0 = 0;
#endif

    if (breakDetected)
    {
        switch(synchState) {
            case (SS_BINARY_SEARCH):
            {
                if ( cycleCount > COUNT_HIGH_LIMIT)
                {
                    OSCCAL -= calStep;
                    NOP();
                }
                else if ( cycleCount < COUNT_LOW_LIMIT)
                {
                    OSCCAL += calStep;
                    NOP();
                }
                else
                {
                    // Within limits, do nothing.
                }
                calStep >>= 1;   // Divide by 2.

                if (calStep == 0)
                {
                    // Binary search complete. Clean up, and exit.

                    breakDetected = FALSE;

                    // Enable UART receiver.
                    SYNCH_USART_STATCTRL_REG_B |= (1 << SYNCH_RXEN);

                    // Disable INT0
                    EXT_INT_MASK_REGISTER &= ~(1 << INT0);
                }
                else
                {
                    //Set external interrupt 0 to trigger on falling edge.
                    EXT_INT_SENSE_CTRL_REGISTER |= (1 << ISC01);
                    EXT_INT_SENSE_CTRL_REGISTER &= ~(1 << ISC00);
                    EXT_INT_FLAG_REGISTER = (1 << INTF0);

                    synchState = SS_MEASURING;
                }
                break;
            }
            case (SS_MEASURING):
            {
                //Set external interrupt 0 to trigger on rising edge.
                EXT_INT_SENSE_CTRL_REGISTER |= (1 << ISC01) | (1 << ISC00);
                EXT_INT_FLAG_REGISTER = (1 << INTF0);

                synchState = SS_BINARY_SEARCH;
                break;
            }
        }
        return;
    }
    else  // breakDetected = FALSE
    {
        // Disable sleep flag. (Ensures that the device
        // does not enter any sleep mode unintended.)
        MCUCR &= ~(1 << SE);
        PREPARE_FOR_SYNCH();
        return;
    }
}

#endif
