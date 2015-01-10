/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.ORG> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.        Joerg Wunsch
 * ----------------------------------------------------------------------------
 *
 * Simple AVR demonstration.  Controls a LED that can be directly
 * connected from OC1/OC1A to GND.  The brightness of the LED is
 * controlled with the PWM.  After each period of the PWM, the PWM
 * value is either incremented or decremented, that's all.
 *
 * $Id: demo.c 1637 2008-03-17 21:49:41Z joerg_wunsch $
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define TIMER1_TOP 0x3FF

enum { UP, DOWN };

ISR (TIMER1_OVF_vect)
{
    static uint16_t pwm;
    static uint16_t extra_counter = 0;
    static uint8_t direction;
    static const soft_scaler = 5;

    switch (direction)
    {
        case UP:
            if ((++extra_counter % soft_scaler) == 0)
            {
                if (++pwm == TIMER1_TOP)
                    direction = DOWN;
            }
            break;

        case DOWN:
            if ((--extra_counter % soft_scaler) == 0)
            {
                if (--pwm == 0)
                    direction = UP;
            }
            break;
    }

    OCR1A = pwm;
}

void
ioinit (void)
{
    /* Enable PORTB 1 as output. */
    DDRB |= _BV(DDB1);

    /*************************************
     * Set up of Timer 1 for Fast PWM mode
     *************************************/

    /* Initial output value */
    OCR1A = 0x0001;

    /* Set none-inverting mode */
    TCCR1A |= _BV(COM1A1);

    /* Set PWM for 10 bit mode */
    TCCR1A |= _BV(WGM11) | _BV(WGM10);
    TCCR1B |= _BV(WGM12);

    /*
     * Start timer 1.
     */
    TCCR1B |= _BV(CS10);

    /*************************************
     * Set up of Timer 0 for CTC mode
     *************************************/
    
    /* Enable Timer overflow interrupt */
    TIMSK1 = _BV(TOIE1);
    sei();
}

int
main (void)
{

    ioinit ();

    /* loop forever, the interrupts are doing the rest */

    while(1)
    {
    }

    return (0);
}
