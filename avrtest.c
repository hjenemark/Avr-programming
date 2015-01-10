/*
 This file is adopted from 
 http://www.nongnu.org/avr-libc/user-manual/group__demo__project.html
 This version is specifically aimed at the Atmega328p as it is on the Arduino
 hardware platform. The mentioned LED is connected to the PIN 9 in the Arduino PCB.
 
 The result is a up/down fading LED.
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
