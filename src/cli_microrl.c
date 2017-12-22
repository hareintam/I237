#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr-uart/uart.h"
#include "hmi_msg.h"
#include "print_helper.h"
#include "cli_microrl.h"

#define NUM_ELEMS(x)        (sizeof(x) / sizeof((x)[0]))
#define UART_STATUS_MASK    0x00FF

void cli_print_help(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_ascii_tbls(const char *const *argv);
void cli_handle_number(const char *const *argv);
void cli_handle_example(const char *const *argv);
void cli_print_cmd_error(void);
void cli_print_cmd_arg_error(void);
int cli_execute(int argc, const char *const *argv);

typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;

const char help_cmd[] PROGMEM = "help";
const char help_help[] PROGMEM = "Get help";
const char ver_cmd[] PROGMEM = "version";
const char ver_help[] PROGMEM = "Print FW version";
const char example_cmd[] PROGMEM = "example";
const char example_help[] PROGMEM =
    "Prints out all provided 3 arguments Usage: example <argument> <argument> <argument>";
const char ascii_cmd[] PROGMEM = "ascii";
const char ascii_help[] PROGMEM = "print ASCII tables";
const char number_cmd[] PROGMEM = "number";
const char number_help[] PROGMEM =
    "Print and display matching number Usage: number <decimal number>";

const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {example_cmd, example_help, cli_handle_example, 3},
    {ascii_cmd, ascii_help, cli_print_ascii_tbls, 0},
    {number_cmd, number_help, cli_handle_number, 1}
};

void cli_print_help(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(PSTR("\n\r"));
    uart0_puts_p(PSTR("Implemented commands:\n\r"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        uart0_puts_p(cli_cmds[i].cmd);
        uart0_puts_p(PSTR(" : "));
        uart0_puts_p(cli_cmds[i].help);
        uart0_puts_p(PSTR("\n\r"));
    }
}

void cli_print_ver(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(PSTR(VER_FW));
    uart0_puts_p(PSTR(VER_LIBC));
}

void cli_handle_example(const char *const *argv)
{
    uart0_puts_p(PSTR("Command had following arguments:\r\n"));

    for (uint8_t i = 1; i < 4; i++) {
        uart0_puts(argv[i]);
        uart0_puts_p(PSTR("\r\n"));
    }
}

void cli_print_ascii_tbls(const char *const *argv)
{
    (void) argv;
    print_ascii_tbl();
    unsigned char ascii_tbl[128];

    for (uint8_t i = 0; i < 128; i++) {
        ascii_tbl[i] = i;
    }
    
    print_for_human(ascii_tbl, NUM_ELEMS(ascii_tbl));
}

void cli_handle_number(const char *const *argv)
{
    for (size_t i = 0; i < strlen(argv[1]); i++) {
        if (!isdigit(argv[1][i])) {
            uart0_puts_p(PSTR("Argument is not a decimal number!\r\n"));
            return;
        }
    }

    lcd_clr(LCD_ROW_2_START, LCD_VISIBLE_COLS);
    lcd_goto(LCD_ROW_2_START);
    int input = atoi(argv[1]);

    if (input >= 0 && input < 10) {
        lcd_puts_P((PGM_P)pgm_read_word(&(numbers[input])));
        uart0_puts_p(PSTR("You entered number "));
        uart0_puts_p((PGM_P)pgm_read_word(&(numbers[input])));
        uart0_puts(PSTR("\n\r"));
    } else {
        uart0_puts_p(PSTR("Please enter number between 0 and 9!\n\r"));
    }
}

void cli_print_cmd_error(void)
{
    uart0_puts_p(PSTR("\n\r"));
    uart0_puts_p(PSTR("Command not implemented.\n\r Use <help> to get help.\n\r"));
}

void cli_print_cmd_arg_error(void)
{
    uart0_puts_p(PSTR("\n\r"));
    uart0_puts_p(
        PSTR("To few or to many arguments for this command\n\rUse <help>\n\r"));
}

int cli_execute(int argc, const char *const *argv)
{
    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            // Test do we have correct arguments to run command
            // Function arguments count shall be defined in struct
            if ((argc - 1) != cli_cmds[i].func_argc) {
                cli_print_cmd_arg_error();
                return 0;
            }

            // Hand argv over to function pointer,
            // cross fingers and hope that funcion handles it properly
            cli_cmds[i].func_p (argv);
            return 0;
        }
    }
    cli_print_cmd_error();
    return 0;
}
