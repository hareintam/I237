#include <stdio.h>
#include <time.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "print_helper.h"
#include "cli_microrl.h"
#include "hmi_msg.h"

#define LED_GREEN       PORTA2 // Arduino Mega digital pin 24
#define UART_BAUD           9600
#define UART_STATUS_MASK    0x00FF

// Create microrl object and pointer on it
microrl_t rl;
microrl_t *prl = &rl;

static inline void init_leds(void)
{
    /* Set port B pin 7 for output and set it low */
    DDRB |= _BV(DDB7);
    PORTB &= ~_BV(PORTB7);
    /* RGB LED board set up and off */
    DDRA |= _BV(LED_GREEN);
    PORTA &= ~_BV(LED_GREEN);
}

static inline void init_lcd(void)
{
    lcd_init();
    lcd_puts_P((PGM_P)pgm_read_word(&stud_name[0]));
    lcd_putc(0xE1);
    lcd_puts_P((PGM_P)pgm_read_word(&stud_name[1]));
    lcd_goto(LCD_ROW_2_START);
}

static inline void init_clicon(void)
{
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart0_puts_p((PGM_P)pgm_read_word(&stud_name[0]));
    uart0_putc(0xE4);
    uart0_puts_p((PGM_P)pgm_read_word(&stud_name[1]));
    uart0_puts_p(PSTR("\r\n"));
    uart0_puts_p(PSTR("Use backspace to delete entry and enter to confirm.\r\n"));
    uart0_puts_p(PSTR("Arrow key's and del do not work currently.\r\n"));
    // Call init with ptr to microrl instance and print callback
    microrl_init(prl, uart0_puts);
    // Set callback for execute
    microrl_set_execute_callback(prl, cli_execute);
}

static inline void init_errcon(void)
{
    uart1_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart1_puts_p(PSTR("Console started\r\n"));
    uart1_puts_p(PSTR(VER_LIBC));
    uart1_puts_p(PSTR(VER_FW));
}

static inline void init_sys_timer(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12); // Turn on CTC (Clear Timer on Compare)
    TCCR1B |= _BV(CS12); // fCPU/256
    OCR1A = 62549; // Note that it is actually two registers OCR5AH and OCR5AL
    TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
}

static inline void heartbeat(void)
{
    static time_t prev_time;
    char ascii_buf[11] = {0x00};
    time_t now = time(NULL);

    if (prev_time != now) {
        //Print uptime to errcon
        ltoa(now, ascii_buf, 10);
        uart1_puts_p(PSTR("Uptime: "));
        uart1_puts(ascii_buf);
        uart1_puts_p(PSTR(" s.\r\n"));
        //Toggle LED
        PORTA ^= _BV(LED_GREEN);
        prev_time = now;
    }
}

void main(void)
{
    init_leds();
    init_errcon();
    init_clicon();
    init_lcd();
    init_sys_timer();
    sei();

    while (1) {
        heartbeat();
        // CLI commands are handled in cli_execute()
        microrl_insert_char(prl, (uart0_getc() & UART_STATUS_MASK));
    }
}

ISR(TIMER1_COMPA_vect)
{
    system_tick();
}
