#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000
#define BLINK_DELAY_BETWEEN_LIGHTS_MS 500

void main (void)
{
    /* Set port A pin 22, 24 and 26 for output for red, green and blue LEDs */
    DDRA |= _BV(DDA0);
    DDRA |= _BV(DDA2);
    DDRA |= _BV(DDA4);
    /* Set port B pin 7 for output and set it low */
    DDRB |= _BV(DDB7);
    PORTB &= ~_BV(PORTB7);

    while (1) {
        /* Set port A pin 22 high to turn red LED on */
        PORTA |= _BV(PORTA0);
        _delay_ms(BLINK_DELAY_MS);
        /* Set port A pin 22 low to turn red LED off */
        PORTA &= ~_BV(PORTA0);
        _delay_ms(BLINK_DELAY_BETWEEN_LIGHTS_MS);
        /* Set port A pin 24 high to turn green LED on */
        PORTA |= _BV(PORTA2);
        _delay_ms(BLINK_DELAY_MS);
        /* Set port A pin 24 low to turn green LED off */
        PORTA &= ~_BV(PORTA2);
        _delay_ms(BLINK_DELAY_BETWEEN_LIGHTS_MS);
        /* Set port A pin 26 high to turn blue LED on */
        PORTA |= _BV(PORTA4);
        _delay_ms(BLINK_DELAY_MS);
        /* Set port A pin 26 low to turn blue LED off */
        PORTA &= ~_BV(PORTA4);
        _delay_ms(BLINK_DELAY_BETWEEN_LIGHTS_MS);
    }
}