/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief
 *      Generates the master synchronization signal.
 *
 *      This contains code that will generate a BREAK/SYNCH signal as described
 *      in the application note. It can be run on a second AVR to act as a
 *      master device to test synchronization.\n
 *      The signal transmitted on the bus will consist of: a BREAK signal, the
 *      specified number of SYNCH bytes, and one byte of data starting at 0 and
 *      increasing by one for each transmission. The last data byte can be used
 *      to confirm that the slave device is able to receive data correctly after
 *      synchronization. The signal is available on the TXD pin of the "master"
 *      device.
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
 * $RCSfile: test.c,v $
 * $Date: 2006/02/17 11:47:59 $  \n
 ******************************************************************************/

#define UART_BAUD_RATE_REG    25

// NUM_SYNCH_BYTES
// Use 1 for single SYNCH byte synchronization method, 2 for double SYNCH byte.
#define NUM_SYNCH_BYTES       1


#include <ioavr.h>
#include <inavr.h>

void main(void)
{
    // Output high signal (idle) on TXD pin
    DDRD = 0xff;
    PORTD = 0xff;
    UBRRH = UART_BAUD_RATE_REG >> 8;
    UBRRL = UART_BAUD_RATE_REG & 0x00ff;

    TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10);
    TIMSK = (1 << TOIE1);

    __enable_interrupt();
    for (;;)
    {

    }
}


#pragma vector=TIMER1_OVF_vect
__interrupt void Generate_signal(void)
{
        unsigned char i;
        static unsigned char j = 0;
        //Output long low signal ( > 13 bit times)
        PORTD = 0x00;
        __delay_cycles(30000);

        //Output short high signal (break, > 1 bit time)
        PORTD = 0xff;
        __delay_cycles(1000);

        for (i = 0; i < NUM_SYNCH_BYTES; i++)
        {
            // Set up UART and transmit 0x55
            UCSRB = (1 << TXEN);
            while ( !( UCSRA & (1<<UDRE)) )
            {

            }
            UDR = 0x55;
        }
        // Transmit one byte of data.
        while ( !( UCSRA & (1<<UDRE)) )
        {

        }
        UDR = j++;
        while ( !( UCSRA & (1<<UDRE)) )
        {

        }
        // Disable UART
        UCSRB &= ~(1 << TXEN);

        // Output high signal (idle) on TXD
        PORTD = 0xff;

}

