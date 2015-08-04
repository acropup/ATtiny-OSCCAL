/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Device specific definitions.
 *
 *      Device specific differences are handled in this file. It should not
 *      be necessary to change the contents of this file, unless a different
 *      Timer/counter or UART module is desired or support for a new device is
 *      needed. \n
 *      For this file to function properly, a symbol describing the targeted
 *      device must be defined at compilation time. In IAR Embedded Workbench
 *      this is done automatically by selecting a device in "Processor
 *      configuration" in the "Project/Options" menu.
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
 * $RCSfile: device_specific.h,v $
 * $Date: 2006/02/17 11:47:59 $  \n
 ******************************************************************************/

// ***********************************************************************
// Device specific defines
// ***********************************************************************
#if !defined(_DEVICE_SPECIFIC_H_)
#define _DEVICE_SPECIFIC_H_

#if defined(__AT90Tiny2313__) | defined(__ATtiny2313__)

#define PORT_INT0                          PORTD
#define DDR_INT0                           DDRD
#define PIN_NUMBER_INT0                    PORTD0
#define EXT_INT_MASK_REGISTER              GIMSK
#define EXT_INT_SENSE_CTRL_REGISTER        MCUCR
#define EXT_INT_FLAG_REGISTER              EIFR
#define SYNCH_EXT_INT_vect                 INT0_vect

#define SYNCH_TIMER_PRESCALER_REGISTER     TCCR0B
#define SYNCH_TIMER_INT_FLAG_REGISTER      TIFR

#define SYNCH_USART_RXC_vect               USART0_RX_vect
#define SYNCH_USART_STATCTRL_REG_A         UCSRA
#define SYNCH_USART_STATCTRL_REG_B         UCSRB
#define SYNCH_UBRRH                        UBRRH
#define SYNCH_UBRRL                        UBRRL
#define SYNCH_RXEN                         RXEN
#define SYNCH_RXCIE                        RXCIE
#define SYNCH_UDR                          UDR
#define SYNCH_FE                           FE

#define EEPROM_WRITE_ENABLE                EEPE

#define SET_OC1A_DIRECTION()  (DDRB |= (1 << PB3))

#define OSCCAL_RESOLUTION                  7

#if defined(NINE_BIT_TIMER)
#define COUNTER_READ_DELAY    22
#else
#define COUNTER_READ_DELAY    3
#endif

#endif


#if defined(__AT90Mega8__) | defined(__ATmega8__) | \
    defined(__AT90Mega16__) | defined(__ATmega16__) | \
    defined(__AT90Mega32__) | defined(__ATmega32__)

#define PORT_INT0                          PORTD
#define DDR_INT0                           DDRD
#define PIN_NUMBER_INT0                    2
#define EXT_INT_MASK_REGISTER              GICR
#define EXT_INT_SENSE_CTRL_REGISTER        MCUCR
#define EXT_INT_FLAG_REGISTER              GIFR
#define SYNCH_EXT_INT_vect                 INT0_vect

#define SYNCH_TIMER_PRESCALER_REGISTER     TCCR0
#define SYNCH_TIMER_INT_FLAG_REGISTER      TIFR

#define SYNCH_USART_RXC_vect               USART_RXC_vect
#define SYNCH_USART_STATCTRL_REG_A         UCSRA
#define SYNCH_USART_STATCTRL_REG_B         UCSRB
#define SYNCH_UBRRH                        UBRRH
#define SYNCH_UBRRL                        UBRRL
#define SYNCH_RXEN                         RXEN
#define SYNCH_RXCIE                        RXCIE
#define SYNCH_UDR                          UDR
#define SYNCH_FE                           FE

#define EEPROM_WRITE_ENABLE                EEWE

#if defined(__AT90Mega16__) | defined(__ATmega16__) | \
    defined(__AT90Mega32__) | defined(__ATmega32__)
#define SET_OC1A_DIRECTION()  (DDRD |= (1 << PD5))
#else
#define SET_OC1A_DIRECTION()  (DDRB |= (1 << PB1))
#endif

#define OSCCAL_RESOLUTION                  8

#if defined(NINE_BIT_TIMER)
#define COUNTER_READ_DELAY    19
#else
#define COUNTER_READ_DELAY    3
#endif

#endif


#if defined(__AT90Mega48__) | defined(__ATmega48__) | \
    defined(__AT90Mega88__) | defined(__ATmega88__) | \
    defined(__AT90Mega168__) | defined(__ATmega168__)

#define PORT_INT0                          PORTD
#define DDR_INT0                           DDRD
#define PIN_NUMBER_INT0                    2
#define EXT_INT_MASK_REGISTER              EIMSK
#define EXT_INT_SENSE_CTRL_REGISTER        EICRA
#define EXT_INT_FLAG_REGISTER              EIFR
#define SYNCH_EXT_INT_vect                 INT0_vect

#define SYNCH_TIMER_PRESCALER_REGISTER     TCCR0B
#define SYNCH_TIMER_INT_FLAG_REGISTER      TIFR0

#define SYNCH_USART_RXC_vect               USART_RX_vect
#define SYNCH_USART_STATCTRL_REG_A         UCSR0A
#define SYNCH_USART_STATCTRL_REG_B         UCSR0B
#define SYNCH_UBRRH                        UBRR0H
#define SYNCH_UBRRL                        UBRR0L
#define SYNCH_RXEN                         RXEN0
#define SYNCH_RXCIE                        RXCIE0
#define SYNCH_UDR                          UDR0
#define SYNCH_FE                           FE0

#define OSCCAL_RESOLUTION                  7

#define EEPROM_WRITE_ENABLE                EEPE

#define SET_OC1A_DIRECTION()  (DDRB |= (1 << PB1))

#if defined(NINE_BIT_TIMER)
#define COUNTER_READ_DELAY    17
#else
#define COUNTER_READ_DELAY    3
#endif

#endif


#if defined(__AT90Mega169__) | defined(__ATmega169__)
/* Make sure to change the line commented futher down when using ATmega169P to
* fit the new oscillator version, as the OSCCAL register is 8 bit, instead of 7
* in ATmega169 revision A to E.*/

#define PORT_INT0                          PORTD
#define DDR_INT0                           DDRD
#define PIN_NUMBER_INT0                    1
#define EXT_INT_MASK_REGISTER              EIMSK
#define EXT_INT_SENSE_CTRL_REGISTER        EICRA
#define EXT_INT_FLAG_REGISTER              EIFR
#define SYNCH_EXT_INT_vect                 INT0_vect

#define SYNCH_TIMER_PRESCALER_REGISTER     TCCR0A
#define SYNCH_TIMER_INT_FLAG_REGISTER      TIFR0
#define SYNCH_USART_RXC_vect               USART0_RXC_vect
#define SYNCH_USART_STATCTRL_REG_A         UCSR0A
#define SYNCH_USART_STATCTRL_REG_B         UCSR0B
#define SYNCH_UBRRH                        UBRR0H
#define SYNCH_UBRRL                        UBRR0L
#define SYNCH_RXEN                         RXEN0
#define SYNCH_RXCIE                        RXCIE0
#define SYNCH_UDR                          UDR0
#define SYNCH_FE                           FE0

/* When using ATmega169 revision F and on, change the OSCCAL resolution due
to different OSCCAL registers*/
#define OSCCAL_RESOLUTION                  7
// #define OSCCAL_RESOLUTION                  8

#define EEPROM_WRITE_ENABLE                EEWE

#define SET_OC1A_DIRECTION()  (DDRB |= (1 << PB5))

#if defined(NINE_BIT_TIMER)
#define COUNTER_READ_DELAY    17
#else
#define COUNTER_READ_DELAY    3
#endif

#endif


#if defined(__AT90Mega64__) | defined(__ATmega64__) | \
    defined(__AT90Mega128__) | defined(__ATmega128__)

#define PORT_INT0                          PORTD
#define DDR_INT0                           DDRD
#define PIN_NUMBER_INT0                    0
#define EXT_INT_MASK_REGISTER              EIMSK
#define EXT_INT_SENSE_CTRL_REGISTER        EICRA
#define EXT_INT_FLAG_REGISTER              EIFR
#define SYNCH_EXT_INT_vect                 INT0_vect

#define SYNCH_TIMER_PRESCALER_REGISTER     TCCR0
#define SYNCH_TIMER_INT_FLAG_REGISTER      TIFR

#define SYNCH_USART_RXC_vect               USART0_RXC_vect
#define SYNCH_USART_STATCTRL_REG_A         UCSR0A
#define SYNCH_USART_STATCTRL_REG_B         UCSR0B
#define SYNCH_UBRRH                        UBRR0H
#define SYNCH_UBRRL                        UBRR0L
#define SYNCH_RXEN                         RXEN0
#define SYNCH_RXCIE                        RXCIE0
#define SYNCH_UDR                          UDR0
#define SYNCH_FE                           FE0

#define OSCCAL_RESOLUTION                  8

#define EEPROM_WRITE_ENABLE                EEWE

#define SET_OC1A_DIRECTION()  (DDRB |= (1 << PB5))

#if defined(NINE_BIT_TIMER)
#define COUNTER_READ_DELAY    19
#else
#define COUNTER_READ_DELAY    3
#endif

#endif

#endif
