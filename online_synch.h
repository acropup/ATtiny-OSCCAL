/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Configuration and macros.
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
 * $RCSfile: online_synch.h,v $
 * $Date: 2006/02/17 11:47:59 $  \n
 ******************************************************************************/

#if !defined(_ONLINE_SYNCH_H_)
#define _ONLINE_SYNCH_H_

// ***********************************************************************
// Defines controlling synchronization
// ***********************************************************************

// Synchronization method. Uncomment one of the following to chose
// Synchronization method.
#define SYNCH_METHOD_SINGLE_SYNCH_BYTE
//#define SYNCH_METHOD_DOUBLE_SYNCH_BYTE

#define TARGET_FREQUENCY      8000000
#define SYNCH_FREQUENCY       19200
#define SYNCH_UBRR            25              // Baud rate register setting to
                                              // obtain SYNCH_FREQUENCY.
#define SYNCH_ACCURACY        10              // 10 equals +/-1% (Only
                                              // for single synch byte method)

// DEFAULT_OSCCAL_ADDRESS: The location in EEPROM where the default OSCCAL
// value can be found. (Only needed for single synch byte method).
#define DEFAULT_OSCCAL_ADDRESS  0x00

// NINE_BIT_TIMER: utilize the overflow bit of Timer/Counter0 as the
// ninth bit. Comment out to use only 8 bits.
#define NINE_BIT_TIMER

// Only for devices with OSCCAL registers with two frequency ranges. Always use
// 0x00 for devices with one continous OSCCAL register.
#define DEFAULT_OSCCAL_MASK   0x00  // Use lower half
//#define DEFAULT_OSCCAL_MASK   0x80  // Use upper half


// This file must be included after user settings
#include "device_specific.h"

// ***********************************************************************
// Precalculated values
// ***********************************************************************
#define TARGET_COUNT      ((TARGET_FREQUENCY / SYNCH_FREQUENCY) - COUNTER_READ_DELAY)
// Applies only to single synch byte method:
#define SYNCH_LIMIT       ((TARGET_FREQUENCY / SYNCH_FREQUENCY) * SYNCH_ACCURACY / 1000)

#if defined(NINE_BIT_TIMER) & (TARGET_COUNT > 511)
#error TARGET_COUNT is larger than 9-bit counter
#elif (!defined(NINE_BIT_TIMER)) & (TARGET_COUNT > 255)
#error TARGET_COUNT is larger than 8-bit counter
#endif

#define COUNT_LOW_LIMIT   (TARGET_COUNT - SYNCH_LIMIT)
#define COUNT_HIGH_LIMIT  (TARGET_COUNT + SYNCH_LIMIT)

#if defined(SYNCH_METHOD_DOUBLE_SYNCH_BYTE)
#define DEFAULT_OSCCAL    ((1 << OSCCAL_RESOLUTION - 1) | DEFAULT_OSCCAL_MASK)
#define INITIAL_STEP      (1 << (OSCCAL_RESOLUTION - 2))
#endif

#if defined(SYNCH_METHOD_SINGLE_SYNCH_BYTE)
#define DEFAULT_OSCCAL      defaultOSCCAL     // Default value read from EEPROM.
#define INITIAL_STEP        (1 << 4)
#endif

// ***********************************************************************
// Predefined symbols
// ***********************************************************************
#define SS_MEASURING        0
#define SS_BINARY_SEARCH    1
#define SS_NEIGHBOR_SEARCH  2

#define FALSE               0
#define TRUE                1


// ***********************************************************************
// Functions implemented as macros to avoid function calls from within
// interrupt service routines.
// ***********************************************************************
#define PREPARE_FOR_SYNCH() \
breakDetected = TRUE; \
synchState = SS_MEASURING; \
calStep = INITIAL_STEP; \
SYNCH_USART_STATCTRL_REG_B &= ~(1 << SYNCH_RXEN); \
EXT_INT_SENSE_CTRL_REGISTER |= (1 << ISC01); \
EXT_INT_SENSE_CTRL_REGISTER &= ~(1 << ISC00); \
EXT_INT_FLAG_REGISTER |= (1 << INTF0); \
EXT_INT_MASK_REGISTER |= (1 << INT0); \
OSCCAL = DEFAULT_OSCCAL; \
NOP();


// For ATmega64 and ATmega128, 8 nop instructions must be run after a
// change in OSCCAL to ensure stability (See errata in datasheet).
// For all other devices, one nop instruction should be run to let
// the RC oscillator stabilize.
// The NOP() macro takes care of this.
#if defined(__AT90Mega64__) | defined(__ATmega64__) | \
    defined(__AT90Mega128__) | defined(__ATmega128__)
#define NOP() \
__no_operation(); \
__no_operation(); \
__no_operation(); \
__no_operation(); \
__no_operation(); \
__no_operation(); \
__no_operation(); \
__no_operation();

#else

#define NOP() __no_operation()

#endif

// Absolute value macro.
#define ABS(var) (((var) < 0) ? -(var) : (var));

// ***********************************************************************
// Function prototypes
// ***********************************************************************
void Initialize_Synchronization( void );

#endif
