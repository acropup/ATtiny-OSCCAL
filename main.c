/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Program entry point and sleep function.
 *
 *      This file contains the main loop of the application and a function
 *      "sleep()". The sleep function is never run in the included code, but
 *      could be used in a protocol implementation to save power while there
 *      is no activity on the bus. It takes care of all the necessary steps to
 *      make the device enter one of the sleep modes and still be prepared to
 *      receive a BREAK signal. By default it will enter "Idle" mode. To enter
 *      a different sleep mode change the SMn bits that are set in MCUCR.
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
 * $Revision: 1.4 $
 * $RCSfile: main.c,v $
 * $Date: 2006/02/21 12:45:48 $  \n
 ******************************************************************************/


#include <inavr.h>
#include <ioavr.h>
#include "online_synch.h"


unsigned char breakDetected;
unsigned char synchState;
unsigned char calStep;

void sleep(void);

void main(void)
{
    // For testing only:
    // Set up Timer/counter1 to generate a frequency of fclk/2 on OC2
    SET_OC1A_DIRECTION();
    OCR1A = 0x00;
    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12) | (1 << CS10);


    Initialize_Synchronization();

    __enable_interrupt();
    for(;;)
    {
    }
}

void sleep(void)
{
    // Set INT0 to trigger on low level.
    __disable_interrupt();
    EXT_INT_SENSE_CTRL_REGISTER &= ~((1 << ISC01) | (1 << ISC00));
    EXT_INT_FLAG_REGISTER |= (1 << INTF0);
    EXT_INT_MASK_REGISTER |= (1 << INT0);

    #if defined(SM2)
    // Set sleep  mode.
    MCUCR |= ((1 << SE) | (1 << SM0) | (0 << SM0) | (0 << SM2));
    #else
    MCUCR |= ((1 << SE) | (1 << SM0) | (0 << SM0));
    #endif

    // Make sure that interrupts are enabled, and go to sleep.
    __enable_interrupt();
    __sleep();
}

/*! \mainpage
* \section Intro Introduction
* This documents data structures, functions, variables, defines, enums, and
* typedefs in the software for application note AVR054. This calibrates the
* internal oscillator run-time via the uart. 50% duty cycle is needed.
*
* \section SCI Compilator Info
* This software was compiled with IAR Embedded Workbench 4.12A. For other
* compilers than IAR modifications are needed.
*
* \section QSG Quick Start Guide
* \subsection SCSC Synchronization source code
* The "syncronization" directory contains the synchronization source code.
* To make a project with IAR EWAVR:\n
* - Create a project in a workspace and add the .c files to the project (main.c,
* single_synch_byte.c and double_synch_byte.c).
* - Select the relevant device, e.g. --cpu=tiny2313, ATtiny2313, enable bit
* definitions in I/O include files, output format: ubrof8 for Debug and
* intel_extended for Release.
* - Select calibration method by uncommenting one of the SYNCH_METHOD_XXXXXX lines.
* - Change TARGET_FREQUENCY to the frequency you want to calibrate the AVR towards.
* - Change SYNCH_FREQUENCY to the frequency of the master SYNCH signal.
* - Change SYNCH_UBRR to the UART Baud Rate Register (UBRR) value needed for the
* UART to communicate at SYNCH_FREQUENCY.
* - If single SYNCH byte synchronization is selected in step 1, change the accuracy
* in frequency needed after synchronization. 10 equals +/-1% of TARGET_FREQUENCY.
* - If single SYNCH byte synchronization is selected, change DEFAULT_OSCCAL_ADDRESS
* to reflect the location in EEPROM of the default OSCCAL value to be loaded on
* every synchronization attempt. Also remember to write the default OSCCAL value
* to EEPROM when programming the chip.
* - Decide if a 9 bit timer is needed. If only 8 bits are needed, comment out
* the line defining NINE_BIT_TIMER.
* - If a device with two frequency ranges is used, it must be decided which one
* is used. (Refer to the data sheet of the device for more information.)
* Uncomment one of the lines defining DEFAULT_OSCCAL_MASK to select range.
*
* The other files do not need to be changed. A brief description is given in
* each file to help understand how an application can be integrated with the
* synchronization source code.
*
* \subsection tstcd Test Code
* The "test" directory contains contains source code that generates a
* BREAK/SYNCH signal for testing. It can be run on a second AVR to act as a
* master device to test synchronization.
* To make a project with IAR EWAVR:\n
* - Create a project in a workspace and add the test.c file to the project.
* - Select the relevant device, e.g. --cpu=mega32, ATmega32, enable bit
* definitions in I/O include files, output format: ubrof8 for Debug and
* intel_extended for Release.
* - In "test.c", change UART_BAUD_RATE_REG to change the baud rate of the
* synchronization signal. See the data sheet for the selected device to find
* the correct UBRR setting at the current frequency.
* - In "test.c", change the value of NUM_SYNCH_BYTES to generate different
* synchronization sequences. NUM_SYNCH_BYTES should be 1 for the single SYNCH
* byte synchronization method and 2 for the double synchronization byte method.
*
* \subsection PTGT Putting it together
* To test the synchronization, program one AVR with the master test software.
* Program a different AVR with the synchronization software. Make sure that the
* fuses of the slave device are configured for internal RC operation at the
* target speed. Connect the TXD pin on the master to RXD and INT0 pins on the
* slave. The main function of the synchronization code sets up Timer1 to output
* a waveform on the OC1A pin equal to half the speed of the internal RC
* oscillator. The frequency of this signal can be measured to verify that the
* synchronization works.
*
*
* \section CSZ Code Size
* The code size depends on the method and timer resolution used. Code size with
* IAR EWAVR 4.12A, ATtiny2313 and no optimization (not including main.c):
* - Single SYNCH byte  8 bits  ~ 360 B
* - Single SYNCH byte  9 bits  ~ 390 B
* - Double SYNCH byte  8 bits  ~ 550 B
* - Double SYNCH byte  9 bits  ~ 560 B
*
*
* \section DI Device Info
* All devices with internal RC oscillator and 1 timer can be used. For supported
* devices see device_specific.h.\n
* The test application is written for ATmega32, but all devices with UART/USART
* can be used.
*
*
* \section contactinfo Contact Info
* For more info about Atmel AVR visit http://www.atmel.com/products/AVR/\n
* For AVR application notes visit
* http://www.atmel.com/dyn/products/app_notes.asp?family_id=607\n
* Support mail: avr@atmel.com
*/
