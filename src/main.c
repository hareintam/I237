#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../lib/hd44780_111/hd44780.h"
#include "uart.h"
#include "print_helper.h"
#include "hmi_msg.h"

#define BLINK_DELAY_MS 100

/* Init error console as stderr in UART1 and print user code info */
static inline void init_errcon(void)
{
    simple_uart1_init();
    stderr = &simple_uart1_out;
    fprintf_P(stderr, PSTR(VER_LIBC));
    fprintf_P(stderr, PSTR(VER_FW));
}

static inline void init_clicon(void)
{
    simple_uart0_init();
    stdout = stdin = &simple_uart0_io;
}

static inline void init_leds(void)
{
    /* Set port A pin 22, 24 and 26 for output for red, green and blue LEDs */
    DDRA |= _BV(DDA0);
    DDRA |= _BV(DDA2);
    DDRA |= _BV(DDA4);
    /* Set port B pin 7 for output and set it low */
    DDRB |= _BV(DDB7);
    PORTB &= ~_BV(PORTB7);
}

static inline void blink_leds(void)
{
    /* Set port A pin 22 high to turn red LED on */
    PORTA |= _BV(PORTA0);
    _delay_ms(BLINK_DELAY_MS);
    /* Set port A pin 22 low to turn red LED off */
    PORTA &= ~_BV(PORTA0);
    /* Set port A pin 24 high to turn green LED on */
    PORTA |= _BV(PORTA2);
    _delay_ms(BLINK_DELAY_MS);
    /* Set port A pin 24 low to turn green LED off */
    PORTA &= ~_BV(PORTA2);
    /* Set port A pin 26 high to turn blue LED on */
    PORTA |= _BV(PORTA4);
    _delay_ms(BLINK_DELAY_MS);
    /* Set port A pin 26 low to turn blue LED off */
    PORTA &= ~_BV(PORTA4);
}

void main(void)
{
    init_leds();
    init_errcon();
    init_clicon();
    /* Print out name to clicon*/
    fprintf_P(stdout, PSTR(NAME_BEGIN));
    fprintf(stdout, "%c", 0xE4);
    fprintf_P(stdout, PSTR(NAME_END "\n"));
    print_ascii_tbl(stdout);
    unsigned char a[128];

    for (int i = 0; i < 128; i++) {
        a[i] = i;
    }

    print_for_human(stdout, a, 128);
    /* Print out name to LCD*/
    lcd_init();
    lcd_puts_P(PSTR(NAME_BEGIN));
    lcd_putc(0xE1);
    lcd_puts_P(PSTR(NAME_END));
    lcd_goto(LCD_ROW_2_START);

    while (1) {
        blink_leds();
        int entered_number;
        fprintf_P(stdout, PSTR(ENTER_NUMBER));
        fscanf(stdin, "%d", &entered_number);
        fprintf(stdout, "%d", entered_number);

        if (entered_number >= 0 && entered_number < 10) {
            fprintf_P(stdout, PSTR(YOUR_NUMBER));
            fprintf_P(stdout, PSTR("%S"), (PGM_P)pgm_read_word(&numbers[entered_number]));
            lcd_goto(LCD_ROW_2_START);
            lcd_puts_P(PSTR(EMPTY_LINE));
            lcd_goto(LCD_ROW_2_START);
            lcd_puts_P((PGM_P)pgm_read_word(&numbers[entered_number]));
        } else {
            fprintf_P(stdout, PSTR(NOT_IN_RANGE));
        }
    }
}
